/*
spacegameengine is a portable easy to use game engine written in C++.
Copyright (C) 2006-2009 Carl Philipp Reh (sefi@s-e-f-i.de)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "height_map/array.hpp"
#include "height_map/scalar.hpp"
#include "height_map/vector2.hpp"
#include "height_map/object.hpp"
#include "height_map/image_to_array.hpp"
#include "graphics/shaders.hpp"
#include "graphics/camera.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/time/timer.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/image/colors.hpp>
#if 0
#include <sge/texture/manager.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/default_creator.hpp>
#include <sge/texture/default_creator_impl.hpp>
#endif
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <sge/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <cstdlib>
#include <exception>
#include <ostream>

int main(int argc,char *argv[])
try
{
	if (argc != 2)
	{
		fcppt::io::cerr << FCPPT_TEXT("usage: ") << fcppt::from_std_string(argv[0]) << FCPPT_TEXT(" <image-file>\n");
		return EXIT_FAILURE;
	}

	fcppt::filesystem::path const filename(
		fcppt::from_std_string(
			argv[1]));

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::window::parameters(
				FCPPT_TEXT("render height map")
			)
		)
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					sge::renderer::screen_size(
						1024,
						768
					),
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care
				),
				sge::renderer::depth_buffer::off,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling
			)
		)
		(
			sge::systems::parameterless::input
		)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions
			)
		)
	);

	insula::height_map::object h(
		sys.renderer(),
		insula::height_map::image_to_array(
			sys.image_loader().load(
				filename)),
		static_cast<insula::height_map::scalar>(
			1000),
		insula::height_map::vector2(
			5,5));

#if 0
	typedef 
	sge::texture::default_creator<sge::texture::no_fragmented> 
	texture_creator;
	
	texture_creator const creator(
		rend,
		sge::image::color::format::rgba8,
		sge::renderer::filter::linear
	);

	sge::texture::manager tex_man(
		rend,
		creator
	);

	sge::texture::const_part_ptr const
		tex1(
			sge::texture::add_image(
				tex_man,
				image_loader.load(
					sge::config::media_path()
					/ FCPPT_TEXT("cloudsquare.jpg")
				)
			)
		);
#endif

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false)));

	sys.renderer()->state(
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::clear_backbuffer = true)
		 	(sge::renderer::state::bool_::enable_lighting = false)
		 	(sge::renderer::state::cull_mode::off)
		 	(sge::renderer::state::depth_func::off)
		 	(sge::renderer::state::draw_mode::line)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));
	
	insula::graphics::shaders shads(
		sys.renderer(),
		FCPPT_TEXT("media/vertex.glsl"),
		FCPPT_TEXT("media/fragment.glsl"));
	
	insula::graphics::camera cam(
		sys.input_system(),
		static_cast<insula::graphics::scalar>(
			1024.0/768.0),
		static_cast<insula::graphics::scalar>(
			fcppt::math::deg_to_rad(
				static_cast<insula::graphics::scalar>(
					90))),
		static_cast<insula::graphics::scalar>(
			1),
		static_cast<insula::graphics::scalar>(
			1000),
		static_cast<insula::graphics::scalar>(
			200));
	
	sge::time::timer frame_timer(
		sge::time::second(
			1));

	while(running)
	{
		sge::mainloop::dispatch();
	
		shads.mvp(
			cam.perspective() * cam.world());

		sge::renderer::scoped_block const block_(
			sys.renderer());

		cam.update(
			frame_timer.reset());

		h.render();
	}
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	fcppt::io::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
