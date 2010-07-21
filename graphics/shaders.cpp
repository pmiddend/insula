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
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>

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

	program_ = 
		renderer->create_glsl_program(
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					vertex_stream)),
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					fragment_stream)));

	renderer->glsl_program(
		program_);
	
	fcppt::io::cout << FCPPT_TEXT("Error log: ") << program_->info_log() << FCPPT_TEXT("\n");
	
	mvp_ = program_->uniform("mvp");
	sand_ = program_->uniform("sand");
	rock_ = program_->uniform("rock");
	grass_ = program_->uniform("grass");

	sge::renderer::glsl::uniform::single_value(
		sand_,
		0);

	sge::renderer::glsl::uniform::single_value(
		rock_,
		1);

	sge::renderer::glsl::uniform::single_value(
		grass_,
		2);
}

sge::renderer::glsl::program_ptr const
insula::graphics::shaders::program()
{
	return program_;
}

void
insula::graphics::shaders::mvp(
	mat4 const &m)
{
	sge::renderer::glsl::uniform::single_value(
		mvp_,
		m);
}
