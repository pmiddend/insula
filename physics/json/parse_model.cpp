#include "parse_model.hpp"
#include "../../model/object.hpp"
#include "../../media_path.hpp"
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/model/loader.hpp>
#include <sge/model/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/io/cifstream.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <memory>
#include <ios>

insula::model::object_ptr const
insula::physics::json::parse_model(
	sge::parse::json::object const &o,
	sge::model::loader_ptr const loader,
	sge::image::multi_loader &il,
	sge::renderer::device_ptr const rend,
	graphics::camera::object &cam,
	graphics::shader &shader)
{
	fcppt::io::cifstream ifs(
		media_path()/
		FCPPT_TEXT("models")/
		FCPPT_TEXT("vehicles")/
		sge::parse::json::find_member_exn<sge::parse::json::string>(
			o.members,
			FCPPT_TEXT("model")),
		std::ios_base::binary);

	sge::model::object_ptr const model_object = 
		loader->load(
			ifs);

	FCPPT_ASSERT_MESSAGE(
		model_object->part_names().size(),
		FCPPT_TEXT("The given model doesn't have any parts!"));

	return 
		std::make_shared<model::object>(
			cam,
			model_object,
			rend,
			shader,
			sge::image::create_texture(
				media_path()/
				FCPPT_TEXT("textures")/
				FCPPT_TEXT("vehicles")/
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					o.members,
					FCPPT_TEXT("texture")),
				rend,
				il,
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none));
}
