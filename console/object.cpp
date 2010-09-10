#include "object.hpp"
#include "../media_path.hpp"
#include <sge/input/system.hpp>
#include <sge/image/colors.hpp>
#include <sge/font/system.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/file.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/config/media_path.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/object.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/clog.hpp>
#include <fcppt/text.hpp>
#include <functional>

insula::console::object::object(
	sge::input::system_ptr const is,
	sge::renderer::device_ptr const rend,
	sge::font::system_ptr const fs,
	sge::image::multi_loader &il,
	redirect_mode::type const redirect)
:
	object_(
		FCPPT_TEXT('/')),
	gfx_(
		object_,
		rend,
		sge::image::colors::black(),
		fs->create_font(
			sge::config::media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				15)),
		is,
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						rend->create_texture(
							il.load(
								media_path() / FCPPT_TEXT("console_back.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
          rend->screen_size().w(),
          static_cast<
            sge::console::sprite_object::unit
          >(
            rend->screen_size().h() / 2)))
      .elements()),
		static_cast<sge::console::output_line_limit>(
			1000)),
	cout_(
		redirect == redirect_mode::all 
		? 
			unique_scoped_streambuf(new streambuf(fcppt::io::cout,object_))
		:
			unique_scoped_streambuf()),
	clog_(
		redirect == redirect_mode::all 
		? 
			unique_scoped_streambuf(new streambuf(fcppt::io::clog,object_))
		:
			unique_scoped_streambuf()),
	cerr_(
		redirect == redirect_mode::all 
		? 
			unique_scoped_streambuf(new streambuf(fcppt::io::cerr,object_))
		:
			unique_scoped_streambuf())
{
}

void
insula::console::object::render()
{
	if (gfx_.active())
		gfx_.draw();
}

sge::console::object &
insula::console::object::model()
{
	return object_;
}

sge::console::object const &
insula::console::object::model() const
{
	return object_;
}

sge::console::gfx &
insula::console::object::view()
{
	return gfx_;
}

sge::console::gfx const &
insula::console::object::view() const
{
	return gfx_;
}

insula::console::object::~object() {}

