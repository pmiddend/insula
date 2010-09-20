#include "object.hpp"
#include "../../exception.hpp"
#include "value_type_impl.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/optional_string.hpp>
#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/no_program.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/no_texture.hpp>
#include <sge/renderer/texture.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/io/cifstream.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/from_std_string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <streambuf>
#include <algorithm>

namespace
{
sge::renderer::glsl::string const
file_to_string(
	fcppt::filesystem::path const &p)
{
	fcppt::io::cifstream stream(
		p);
	return 
		sge::renderer::glsl::string(
			std::istreambuf_iterator<sge::renderer::glsl::string::value_type>(
				stream),
			std::istreambuf_iterator<sge::renderer::glsl::string::value_type>());
}

// TODO: Replace by polymorphic lambda
struct uniform_setter
{
public:
	// apply_unary needs this 
	typedef void result_type;

	sge::renderer::glsl::uniform::variable_ptr v;

	explicit
	uniform_setter(
		sge::renderer::glsl::uniform::variable_ptr const v)
	:
		v(v)
	{
	}

	template<typename T>
	result_type
	operator()(T const &t) const
	{
		sge::renderer::glsl::uniform::single_value(
			v,
			t);
	}
};
}

insula::graphics::shader::object::object(
	sge::renderer::device_ptr _renderer,
	fcppt::filesystem::path const &vertex,
	fcppt::filesystem::path const &fragment,
	fcppt::string const &format_declaration,
	variable_sequence const &_variables,
	sampler_sequence const &_samplers)
:
	renderer_(
		_renderer)
{
	if (!fcppt::filesystem::exists(vertex))
		throw exception(
			vertex.string()+FCPPT_TEXT(" doesn't exist!"));
	
	if (!fcppt::filesystem::exists(fragment))
		throw exception(
			fragment.string()+FCPPT_TEXT(" doesn't exist!"));

	sge::renderer::glsl::string const header = 
		std::accumulate(
			_variables.begin(),
			_variables.end(),
			sge::renderer::glsl::string(),
			[](sge::renderer::glsl::string const &s,variable const &v) 
			{ 
				return s + v.declaration(); 
			})+
		std::accumulate(
			_samplers.begin(),
			_samplers.end(),
			sge::renderer::glsl::string(),
			[](sge::renderer::glsl::string const &s,sampler const &v) 
			{ 
				return s + v.declaration(); 
			});

	/*
	fcppt::io::cout << "Generated header: " << header << "\n";
	fcppt::io::cout << "Got format declaration: " << format_declaration << "\n";
	fcppt::io::cout << "Preprocessed vertex shader: " << 
		boost::algorithm::replace_first_copy(
					file_to_string(
						vertex),
					std::string("$$$HEADER$$$"),
					format_declaration+header) << "\n";
	*/

	program_ = 
		renderer_->create_glsl_program(
			sge::renderer::glsl::optional_string(
				boost::algorithm::replace_first_copy(
					file_to_string(
						vertex),
					std::string("$$$HEADER$$$"),
					format_declaration+header)),
			sge::renderer::glsl::optional_string(
				boost::algorithm::replace_first_copy(
					file_to_string(
						fragment),
					std::string("$$$HEADER$$$"),
					header)));

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		program_);

	//fcppt::io::cout << "Iterating through variables\n";
	BOOST_FOREACH(variable const &v,_variables)
	{
		if (v.type() != variable_type::uniform)
			continue;

	//	fcppt::io::cout << "Setting initial value for variable " << v.name() << "\n";
		// TODO: See above
		fcppt::variant::apply_unary(
			uniform_setter(
				uniforms_.insert(
					uniform_map::value_type(
						v.name(),
						program_->uniform(v.name()))).first->second),
			v.initial_value());
	}
	//fcppt::io::cout << "Iterating through variables - DONE\n";

	//fcppt::io::cout << "Now iterating though samplers\n";
	sampler::texture_unit_type current_tu = 
		static_cast<sampler::texture_unit_type>(0);
	BOOST_FOREACH(
		sampler const &v,
		_samplers)
	{
		samplers_.push_back(
			v);

	/*	fcppt::io::cout 
			<< "Assigning " << v.name() << " the texture unit " << current_tu << "\n";*/

		sge::renderer::glsl::uniform::single_value(
			uniforms_.insert(
				uniform_map::value_type(
					v.name(),
					program_->uniform(v.name()))).first->second,
			current_tu);

		samplers_.back().texture_unit(
			current_tu);

		++current_tu;
	}
	
	if (!program_->info_log().empty())
	{
		fcppt::io::cout 
			<< FCPPT_TEXT("Error log: ") 
			<< program_->info_log() 
			<< FCPPT_TEXT("\n");
	}
}

void
insula::graphics::shader::object::set_uniform(
	sge::renderer::glsl::string const &name,
	value_type const &v)
{
	uniform_map::iterator const i = 
		uniforms_.find(name);

	if (i == uniforms_.end())
		throw exception(
			FCPPT_TEXT("Uniform ")+
			fcppt::from_std_string(
				name)+
			FCPPT_TEXT(" was not found"));

	fcppt::variant::apply_unary(
		uniform_setter(
			i->second),
		v);
}

void
insula::graphics::shader::object::update_texture(
	sge::renderer::glsl::string const &name,
	sge::renderer::texture_base_ptr const newtex)
{
	BOOST_FOREACH(
		sampler_sequence::reference r,
		samplers_)
	{
		if (r.name() == name)
		{
			r.texture(
				newtex);
			return;
		}
	}
	FCPPT_ASSERT(false);
}

sge::renderer::glsl::program_ptr const
insula::graphics::shader::object::program()
{
	return program_;
}

void
insula::graphics::shader::object::activate()
{
	renderer_->glsl_program(
		program_);
	
	BOOST_FOREACH(
		sampler_sequence::const_reference r,
		samplers_)
		renderer_->texture(
			r.texture(),
			r.texture_unit());
}

void
insula::graphics::shader::object::deactivate()
{
	renderer_->glsl_program(
		sge::renderer::glsl::no_program());
	BOOST_FOREACH(
		sampler_sequence::const_reference r,
		samplers_)
		renderer_->texture(
			sge::renderer::no_texture(),
			r.texture_unit());
}

insula::graphics::shader::object::~object()
{
}
