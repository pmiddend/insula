#include "graphics/scalar.hpp"
#include "graphics/vec2.hpp"
#include "graphics/shader/variable.hpp"
#include "graphics/shader/value_type_impl.hpp"
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vertex_size.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/format.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/io/cout.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace
{
namespace vf_tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(texcoord)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		3
	>,
	vf_tags::position
> 
vf_position;

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		2
	>,
	vf_tags::texcoord
> 
vf_texcoord;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		vf_position,
		vf_texcoord
	>
> 
vertex_format;
}

template<typename T,sge::renderer::vf::vertex_size N>
fcppt::string const
vec_to_string()
{
	return FCPPT_TEXT("vec")+fcppt::lexical_cast<fcppt::string>(N);
}

//template<typename T,typename Tag,sge::renderer::vf::vertex_size N>



struct format_printer
{
	fcppt::string &s;

	explicit 
	format_printer(
		fcppt::string &s) : s(s) {}

	template<typename T,typename Tag,sge::renderer::vf::vertex_size N>
	void
	operator()(
		sge::renderer::vf::unspecified
		<
			sge::renderer::vf::vector<T,N>,Tag>)
	{
		s += 
			(fcppt::format(FCPPT_TEXT("in %s %s;\n"))
			% vec_to_string<T,N>()
			% Tag::name()).str();
	}

	template<typename T>
	void
	operator()(T const &t) const
	{
	}
};

template<typename Format>
fcppt::string const
vf_to_string()
{
	fcppt::string s;
	boost::mpl::for_each<typename Format::elements>(
		format_printer(
			s));
	return s;
}

int main()
{
	fcppt::string s;

	fcppt::io::cout 
		<< FCPPT_TEXT("Testing vec_to_string:\n")
		<< 
			vec_to_string
			<
				vf_position::subelement_type,
				vf_position::num_subelements
			>() 
		<< FCPPT_TEXT("\n");

	fcppt::io::cout
		<< FCPPT_TEXT("Testing format_printer:\n")
		<< vf_to_string<vertex_format>()
		<< FCPPT_TEXT("\n");

	fcppt::io::cout <<
		insula::graphics::shader::variable(
			FCPPT_TEXT("hier_name"),
			insula::graphics::shader::variable_type::const_,
			insula::graphics::vec2(1,2)).declaration() << FCPPT_TEXT("\n");
}
