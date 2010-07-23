#include "shader.hpp"
#include <sge/renderer/glsl/program_ptr.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/istream_ref.hpp>
#include <sge/renderer/glsl/optional_istream.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/io/cifstream.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>

insula::graphics::shader::shader(
	sge::renderer::device_ptr const _renderer,
	fcppt::filesystem::path const &vertex,
	fcppt::filesystem::path const &fragment)
:
	renderer_(
		_renderer)
{
	fcppt::io::cifstream 
		vertex_stream(
			vertex),
		fragment_stream(
			fragment);

	program_ = 
		renderer_->create_glsl_program(
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					vertex_stream)),
			sge::renderer::glsl::optional_istream(
				sge::renderer::glsl::istream_ref(
					fragment_stream)));
	
	fcppt::io::cout 
		<< FCPPT_TEXT("Error log: ") 
		<< program_->info_log() 
		<< FCPPT_TEXT("\n");
}

void
insula::graphics::shader::activate()
{
	renderer_->glsl_program(
		program_);
}