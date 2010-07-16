#include "shaders.hpp"
#include <sge/renderer/glsl/program_ptr.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/istream_ref.hpp>
#include <sge/renderer/glsl/optional_istream.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/io/cifstream.hpp>

insula::graphics::shaders::shaders(
	sge::renderer::device_ptr const renderer,
	fcppt::filesystem::path const &vertex,
	fcppt::filesystem::path const &fragment)
{
	fcppt::io::cifstream 
		vertex_stream(
			vertex),
		fragment_stream(
			fragment);

	sge::renderer::glsl::program_ptr const p(
		renderer->create_glsl_program(
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					vertex_stream)),
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					fragment_stream))));

	renderer->glsl_program(
		p);
	
	mvp_ = p->uniform("mvp");
}

void
insula::graphics::shaders::mvp(
	mat4 const &m)
{
	sge::renderer::glsl::uniform::single_value(
		mvp_,
		m);
}
