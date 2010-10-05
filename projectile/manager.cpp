#include "manager.hpp"
#include "manager_parameters.hpp"
#include "../model/object.hpp"
#include "../create_path.hpp"
#include "../physics/rigid/parameters.hpp"
#include "../math/box_radius.hpp"
#include <sge/systems/instance.hpp>
#include <sge/model/loader.hpp>
#include <sge/model/load_flags.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/text.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

insula::projectile::manager::manager(
	manager_parameters const &params)
:
	backend_(
		params.scene_manager,
		// transparency
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
							params.config_file.members,
							FCPPT_TEXT("texture")),
						FCPPT_TEXT("textures")),
					params.systems.renderer(),
					params.systems.image_loader(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none)
			}
		},
		std::make_shared<model::object>(
			params.systems.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						params.config_file.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models")),
				sge::model::load_flags::switch_yz),
			params.systems.renderer())),
	scene_manager_(
		params.scene_manager),
	broadphase_(
		params.broadphase),
	world_(
		params.world),
	mass_(
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("mass")))),
	shape_(
		std::make_shared<btSphereShape>(
			static_cast<physics::scalar>(
				math::box_radius(
					backend_.model().bounding_box()))))
{
}

void
insula::projectile::manager::spawn(
	physics::vec3 const &position,
	physics::vec3 const &linear_velocity)
{
	models_.push_back(
		new rigid_model(
			graphics::mat4::identity(),
			broadphase_,
			physics::rigid::parameters(
				world_,
				physics::object_type::projectile,
				position,
				physics::mat3::identity(),
				shape_,
				physics::rigid::solidity::solid,
				mass_,
				linear_velocity)));
	scene_manager_.insert(
		backend_,
		models_.back());
}
