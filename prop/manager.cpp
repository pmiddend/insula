#include "manager.hpp"
#include "parameters.hpp"
#include "../create_path.hpp"
#include "../random_engine.hpp"
#include "../stdlib/for_each.hpp"
#include "../random_seed.hpp"
#include "../physics/scalar.hpp"
#include "../height_map/random_flat_point.hpp"
#include "../height_map/random_point.hpp"
#include "../height_map/object.hpp"
#include "../height_map/vec2.hpp"
#include "../height_map/height_for_point.hpp"
#include "../model/object.hpp"
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
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/image/create_texture.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/scaling.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/twopi.hpp>
#include <boost/foreach.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btVector3.h>
#include <utility>
#include <random>

namespace
{
template<typename Target,typename Source>
std::pair<Target,Target> const
parse_range(
	sge::parse::json::array const &a)
{
	return 
		std::make_pair(
			static_cast<Target>(
				sge::parse::json::get<Source>(
					a.elements[0])),
			static_cast<Target>(
				sge::parse::json::get<Source>(
					a.elements[1])));
}

template<typename T>
typename 
fcppt::math::matrix::static_<T, 4, 4>::type const
uniform_scaling_matrix(
	T const p)
{
	return 
		fcppt::math::matrix::scaling(
			p,
			p,
			p);
}

template<typename T,fcppt::math::size_type N,typename Source>
typename
fcppt::math::vector::static_<T,N>::type const
parse_vector(
	sge::parse::json::array const &a)
{
	typedef
	typename
	fcppt::math::vector::static_<T,N>::type
	vector;

	vector
	result;

	typename vector::iterator i = 
		result.begin();
	

	BOOST_FOREACH(sge::parse::json::value const &v,a.elements)
		*i++ = 
			static_cast<T>(
				sge::parse::json::get<Source>(
					v));

	return result;
}
}

insula::prop::manager::manager(
	parameters const &params)
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
	model::shared_object_ptr model = 
		std::make_shared<model::object>(
			params.systems.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						p.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models/props"))),
			params.systems.renderer());

	backends_.push_back(
		new model_backend(
			sge::parse::json::find_member_exn<bool>(
				p.members,
				FCPPT_TEXT("has_transparency")),
			params.systems.renderer(),
			params.camera,
			params.model_shader,
			fcppt::assign::make_container<model_backend::texture_map>(
				model_backend::texture_map::value_type(
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
						sge::renderer::resource_flags::none))),
			model));

	// We pull all the settings from the json because we need them in
	// the loop that follows.
	bool const flat_terrain =	
		sge::parse::json::find_member_exn<bool>(
			p.members,
			FCPPT_TEXT("flat_terrain"));	

	physics::solidity::type const solid = 
		sge::parse::json::find_member_exn<bool>(
			p.members,
			FCPPT_TEXT("solid")) 
		? 
			physics::solidity::solid 
		: 
			physics::solidity::nonsolid;

	physics::scalar const penetration_depth = 
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				p.members,
				FCPPT_TEXT("penetration_depth")));

	std::pair<physics::scalar,physics::scalar> const scale_range = 
		parse_range<physics::scalar,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("scale_range")));

	physics::vec3 const rotation_axis = 
		parse_vector<physics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("rotation_axis")));

	physics::vec3 const physics_offset = 
		parse_vector<physics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				p.members,
				FCPPT_TEXT("physics_offset")));

	physics::shared_shape_ptr const shape = 
		parse_shape(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				p.members,
				FCPPT_TEXT("approximation")));

	std::size_t const count = 
		static_cast<std::size_t>(
			sge::parse::json::find_member_exn<sge::parse::json::int_type>(
					p.members,
					FCPPT_TEXT("approximation")));

	// Beginning of the actual algorithm

	random_engine rng_engine(
		random_seed());

	std::uniform_real_distribution<physics::scalar> twopi_rng(
		static_cast<physics::scalar>(0),
		fcppt::math::twopi<physics::scalar>());

	std::uniform_real_distribution<physics::scalar> scale_rng(
		scale_range.first,
		scale_range.second);

	for (std::size_t i = 0; i < count; ++i)
	{
		height_map::vec2 const point2 = 
			flat_terrain
			?
				height_map::random_flat_point(
					params.height_map,
					params.water_level,
					rng_engine)
			:
				height_map::random_point(
					params.height_map,
					params.water_level,
					rng_engine);
			
		blueprints_.push_back(
			new blueprint(
				backends_.back(),
				shape,
				fcppt::math::matrix::rotation_axis(
					twopi_rng(
						rng_engine),
					rotation_axis) * 
				uniform_scaling_matrix(
					scale_rng(
						rng_engine)),
				physics::vec3(
					static_cast<physics::scalar>(
						point2.x()),
					height_map::height_for_point(
						params.height_map.heights(),
						static_cast<height_map::scalar>(
							params.height_map.cell_size()),
						point2) -
					model->bounding_box().bottom() - 
					penetration_depth,
					static_cast<physics::scalar>(
						point2.y())),
				physics_offset,
				solid));
	}
}

insula::physics::shared_shape_ptr const
insula::prop::manager::parse_shape(
	sge::parse::json::object const &o)
{
	return 
		std::make_shared<btBoxShape>(
			btVector3(
				10,
				10,
				10));
}

insula::prop::manager::~manager()
{
}
