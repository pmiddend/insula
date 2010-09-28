#include "manager.hpp"
#include "parameters.hpp"
#include "instance.hpp"
#include "instance_parameters.hpp"
#include "../create_path.hpp"
#include "../exception.hpp"
#include "../json/parse_vector.hpp"
#include "../json/member_has_type.hpp"
#include "../random_engine.hpp"
#include "../stdlib/for_each.hpp"
#include "../random_seed.hpp"
#include "../physics/scalar.hpp"
#include "../physics/vec2.hpp"
#include "../height_map/random_point.hpp"
#include "../height_map/object.hpp"
#include "../height_map/vec2.hpp"
#include "../physics/shape_from_approximation.hpp"
#include "../physics/triangle_mesh/shape_from_model.hpp"
#include "../physics/approximation/string_to_cylinder_orientation.hpp"
#include "../model/object.hpp"
#include "../scene/manager.hpp"
#include "../stdlib/shortest_levenshtein.hpp"
#include <boost/foreach.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/systems/instance.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/image/create_texture.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/scaling.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/twopi.hpp>
#include <boost/foreach.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btVector3.h>
#include <utility>
#include <vector>
#include <random>

insula::prop::manager::manager(
	parameters const &params)
:
	scene_manager_(
		params.scene_manager),
	broadphase_manager_(
		params.broadphase_manager)
{
	stdlib::for_each(
		sge::parse::json::find_member_exn<sge::parse::json::array>(
			params.json_object.members,
			FCPPT_TEXT("props")).elements,
		[&params,this](sge::parse::json::value const &v)
		{
				return 
					this->parse_single_prop(
						params,
						sge::parse::json::get<sge::parse::json::object>(
							v));
		});
}

void
insula::prop::manager::parse_single_prop(
	parameters const &params,
	sge::parse::json::object const &p)
{
	// We need this for the "exact" approximation later
	sge::model::object_ptr const model_raw = 
		params.systems.md3_loader()->load(
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					p.members,
					FCPPT_TEXT("model")),
				FCPPT_TEXT("models/props")),
			sge::model::load_flags::switch_yz);

	// And we need this for the bounding box later
	model::shared_object_ptr model = 
		std::make_shared<model::object>(
			model_raw,
			params.systems.renderer());

	backends_.push_back(
		new model::backend(
			// transparency is removed for now
			false,
			params.systems.renderer(),
			params.camera,
			params.model_shader,
			{
				{
					"texture",
					sge::image::create_texture(
						create_path(
							sge::parse::json::find_member_exn<sge::parse::json::string>(
								p.members,
								FCPPT_TEXT("texture")),
							FCPPT_TEXT("textures/props")),
						params.systems.renderer(),
						params.systems.image_loader(),
						sge::renderer::filter::trilinear,
						sge::renderer::resource_flags::none)
				}
			},
			model));

	// We pull all the settings from the json because we need them in
	// the loop that follows.

	height_map::flatness_range const flatness =	
		json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("flatness_range")));

	physics::scalar const penetration_depth = 
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				p.members,
				FCPPT_TEXT("penetration_depth")));

	physics::vec2 const scale_range = 
		json::parse_vector<physics::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("scale_range")));

	physics::vec3 const rotation_axis = 
		json::parse_vector<physics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("rotation_axis")));

	physics::vec3 const physics_offset = 
		json::parse_vector<physics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("physics_offset")));

	std::size_t const count = 
		static_cast<std::size_t>(
			sge::parse::json::find_member_exn<sge::parse::json::int_type>(
				p.members,
				FCPPT_TEXT("count")));

	// Beginning of the actual algorithm

	random_engine rng_engine(
		random_seed());

	std::uniform_real_distribution<physics::scalar> twopi_rng(
		static_cast<physics::scalar>(0),
		fcppt::math::twopi<physics::scalar>());

	std::uniform_real_distribution<physics::scalar> scale_rng(
		scale_range[0],
		scale_range[1]);

	// Test if the approximation is "exact". If so, create nonscaled
	// prototype shape and give that to the parse_approximation function
	bool const exact_approximation = 
		json::member_has_type<sge::parse::json::string>(
			p,
			FCPPT_TEXT("approximation"));

	if(exact_approximation)
		mesh_prototypes_.push_back(
			physics::triangle_mesh::shape_from_model(
				model_raw));

	for (std::size_t i = 0; i < count; ++i)
	{
		physics::scalar const scaling = 
			scale_rng(
				rng_engine);

		blueprints_.push_back(
			new blueprint(
				backends_.back(),
				exact_approximation
				?
					mesh_prototypes_.back().spawn_scaled_instance(
						scaling)
				:
					physics::shape_from_approximation(
						parse_approximation(
							sge::parse::json::find_member_exn<sge::parse::json::object>(
								p.members,
								FCPPT_TEXT("approximation")),
							scaling)),
				rotation_axis,
				twopi_rng(
					rng_engine),
				scaling,
				fcppt::math::vector::structure_cast<physics::vec3>(
					height_map::random_point(
						params.height_map,
						params.water_level,
						rng_engine,
						flatness))
					+ 
					physics::vec3(
						static_cast<physics::scalar>(
							0),
						model->bounding_box().bottom() * scaling - 
						penetration_depth * scaling,
						static_cast<physics::scalar>(
							0)),
				physics_offset));
	}
}

insula::prop::shared_instance_ptr const
insula::prop::manager::instantiate(
	physics::world &world)
{
	return 
		std::make_shared<instance>(
			instance_parameters(
				world,
				*this));
}

insula::physics::approximation::variant const
insula::prop::manager::parse_approximation(
	sge::parse::json::object const &o,
	physics::scalar const scaling)
{
	fcppt::string const type = 
		sge::parse::json::find_member_exn<sge::parse::json::string>(
			o.members,
			FCPPT_TEXT("type"));

	if (type == FCPPT_TEXT("box"))
		return physics::approximation::box(
			scaling * 
			fcppt::math::vector::structure_cast<physics::dim3>(
				json::parse_vector<physics::scalar,3,sge::parse::json::float_type>(
					sge::parse::json::find_member_exn<sge::parse::json::array>(
						o.members,
						FCPPT_TEXT("size")))));

	if (type == FCPPT_TEXT("sphere"))
		return physics::approximation::sphere(
			scaling * 
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					o.members,
					FCPPT_TEXT("radius"))));

	if (type == FCPPT_TEXT("cylinder"))
		return physics::approximation::cylinder(
			physics::approximation::string_to_cylinder_orientation(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					o.members,
					FCPPT_TEXT("orientation"))),
			scaling * 
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					o.members,
					FCPPT_TEXT("height"))),
			scaling * 
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					o.members,
					FCPPT_TEXT("radius"))));

	std::vector<fcppt::string> const allowed_types
	{
		FCPPT_TEXT("sphere"),
		FCPPT_TEXT("box"),
		FCPPT_TEXT("cylinder"),
		FCPPT_TEXT("exact")
	};

	throw 
		exception(
			FCPPT_TEXT("Invalid approximation \"")+
			type+
			FCPPT_TEXT("\", did you mean \"")+
			stdlib::shortest_levenshtein(
				allowed_types,
				type)+
			FCPPT_TEXT("\"?"));
}

insula::prop::manager::~manager()
{
}
