#include "object.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../graphics/dim3.hpp"
#include "../graphics/rect.hpp"
#include "../media_path.hpp"
#include "../graphics/vec4.hpp"
#include "../stdlib/normalize.hpp"
#include "../stdlib/grid/sobel_operator.hpp"
#include "../stdlib/grid/average_convolve.hpp"
#include "../math/triangle/to_plane.hpp"
#include "vf/packed_normal.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include "vf/height_and_gradient.hpp"
#include "vf/packed_height_and_gradient.hpp"
#include "vf/normal.hpp"
#include "vf/vertex_view.hpp"
#include "scalar.hpp"
#include "array.hpp"
#include "vec2.hpp"
#include "dim2.hpp"
#include "rect.hpp"
#include "plane.hpp"
#include "calculate_index_cell.hpp"
#include "parameters.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/index_buffer.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/index/dynamic/format.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/indexed_primitive_type.hpp>
#include <sge/renderer/primitive_count.hpp>
#include <sge/renderer/first_index.hpp>
#include <sge/renderer/index/i32.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/scoped_texture.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/image/file.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/box/contains_point.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <boost/foreach.hpp>
#include <type_traits>
#include <cmath>
#include <algorithm>

namespace
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	typedef 
	insula::height_map::array::size_type
	size_type;

	explicit
	index_visitor(
		insula::height_map::array::dim const &_dim)
	:
		dim_(
			_dim)
	{
	}

	template<typename T>
	result_type
	operator()(
		T const &t) const
	{
		typedef typename 
		T::value_type 
		value_type;

		typename T::iterator it = t.begin();

		for (size_type y = 0; y < static_cast<size_type>(dim_.h()-1); ++y)
			for (size_type x = 0; x < static_cast<size_type>(dim_.w()-1); ++x)
				BOOST_FOREACH(
					value_type const i,
					insula::height_map::calculate_index_cell<value_type>(
						static_cast<value_type>(
							x),
						static_cast<value_type>(
							y),
						static_cast<value_type>(
							dim_.w()),
						static_cast<value_type>(
							dim_.h())))
					*it++ = i;
	}
private:
	insula::height_map::array::dim dim_;
};
}

insula::height_map::object::object(
	parameters const &params)
:
	camera_(
		params.camera),
	renderer_(
		params.renderer),
	cell_size_(
		params.cell_size),
	height_scaling_(
		params.height_scaling),
	vb_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				params.array.size()),
			sge::renderer::resource_flags::none)),
	ib_(
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// q = (n-1)*(m-1) Quads
			// 2*q Tris
			// 3*2*q Indices
			static_cast<sge::renderer::size_type>(
				3*2*((params.array.dimension()[0]-1)*(params.array.dimension()[1]-1))),
			sge::renderer::resource_flags::none)),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("height_map_vertex.glsl"),
		media_path()/FCPPT_TEXT("height_map_fragment.glsl"),
		graphics::shader::vf_to_string<vf::format>(),
		{
			graphics::shader::variable(
				"sun_direction",
				graphics::shader::variable_type::const_,
				normalize(
					params.sun_direction)),
			graphics::shader::variable(
				"ambient_light",
				graphics::shader::variable_type::const_,
				params.ambient_light),
			graphics::shader::variable(
				"texture_scaling",
				graphics::shader::variable_type::const_,
				params.texture_scaling),
			graphics::shader::variable(
				"grid_size",
				graphics::shader::variable_type::const_,
				params.cell_size * 
				fcppt::math::dim::structure_cast<graphics::vec2>(
					params.array.dimension())),
			graphics::shader::variable(
				"do_clip",
				graphics::shader::variable_type::uniform,
				0),
			graphics::shader::variable(
				"water_level",
				graphics::shader::variable_type::uniform,
				static_cast<graphics::scalar>(0)),
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4())
		},
		{
			graphics::shader::sampler(
				"rock",
				renderer_->create_texture(
					params.gradient_texture_image->view(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none)),
			graphics::shader::sampler(
				"grass",
				renderer_->create_texture(
					params.upper_texture_image->view(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none)),
			graphics::shader::sampler(
				"sand",
				renderer_->create_texture(
					params.lower_texture_image->view(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none))
		}),
	extents_(
		graphics::vec3::null(),
		graphics::dim3(
			cell_size_ * 
			static_cast<graphics::scalar>(
				params.array.dimension().w()),
			height_scaling_,
			cell_size_ * 
			static_cast<graphics::scalar>(
				params.array.dimension().h()))),
	heights_(
		stdlib::grid::average_convolve(
			stdlib::grid::average_convolve(
				stdlib::grid::average_convolve(
					params.array)))),
	gradient_(
		stdlib::normalize(
			stdlib::grid::sobel_operator(
				heights_))),
	points_(
		params.array.dimension()),
	normals_(
		params.array.dimension())
{
	// We need the stretched values for the texture layers (_before_ the
	// convolution!)
	array const stretched = 
		stdlib::normalize(
			params.array);

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	// Calculate just the points
	for (array::size_type y = 0; y < heights_.dimension().h(); ++y)
	{
		for (array::size_type x = 0; x < heights_.dimension().w(); ++x)
		{
			points_[vec3_array::dim(x,y)] = 
				vec3(
					static_cast<scalar>(x) * cell_size_,
					height_scaling_ * heights_[array::dim(x,y)],
					static_cast<scalar>(y) * cell_size_);
		}
	}

	std::fill(
		normals_.begin(),
		normals_.end(),
		vec3::null());

	// Calculate the averaged normals
	for (array::size_type y = 1; y < static_cast<array::size_type>(heights_.dimension().h()-1); ++y)
	{
		for (array::size_type x = 1; x < static_cast<array::size_type>(heights_.dimension().w()-1); ++x)
		{
			vec3 const point = 
				points_[vec3_array::dim(x,y)];

			normals_[vec3_array::dim(x,y)] = 
				normalize(
					(cross(
						points_[vec3_array::dim(x+1,y)] - point,
						points_[vec3_array::dim(x,y-1)] - point) + 
					cross(
						points_[vec3_array::dim(x,y-1)] - point,
						points_[vec3_array::dim(x-1,y)] - point) + 
					cross(
						points_[vec3_array::dim(x-1,y)] - point,
						points_[vec3_array::dim(x,y+1)] - point) + 
					cross(
						points_[vec3_array::dim(x,y+1)] - point,
						points_[vec3_array::dim(x+1,y)] - point))/
					static_cast<scalar>(4));
		}
	}

	// And fill the vertex buffer
	for (array::size_type y = 0; y < heights_.dimension().h(); ++y)
	{
		for (array::size_type x = 0; x < heights_.dimension().w(); ++x)
		{
			(*vb_it).set<vf::position>(
				points_[vec3_array::dim(x,y)]);

			(*vb_it).set<vf::normal>(
				normals_[vec3_array::dim(x,y)]);

			(*vb_it).set<vf::height_and_gradient>(
				vf::packed_height_and_gradient(
					stretched[array::dim(x,y)],
					gradient_[array::dim(x,y)]));
			
			vb_it++;
		}
	}
	
	fcppt::variant::apply_unary(
		index_visitor(
			heights_.dimension()),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());
}

void
insula::height_map::object::render(
	sge::renderer::state::cull_mode::type const culling,
	fcppt::optional<graphics::scalar> const &clip_height)
{
	graphics::shader::scoped scoped_shader_(
		shader_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	shader_.set_uniform(
		FCPPT_TEXT("do_clip"),
		// There is no overload for booleans
		static_cast<int>(
			clip_height ? 1 : 0));

	if (clip_height)
	{
		shader_.set_uniform(
			FCPPT_TEXT("water_level"),
			// There is no overload for booleans
			*clip_height);
	}

	renderer_->enable_clip_plane(
		static_cast<sge::renderer::clip_plane_index>(
			0),
		clip_height);
	
	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.perspective() * camera_.rotation() * camera_.translation());
	
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(culling)
		 	(sge::renderer::state::depth_func::less));

	renderer_->render(
		ib_,
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::indexed_primitive_type::triangle,
		sge::renderer::primitive_count(
			ib_->size() / 3),
		sge::renderer::first_index(
			0));
}

insula::graphics::box const
insula::height_map::object::extents() const
{
	return 
		extents_;
}

insula::height_map::scalar
insula::height_map::object::project(
	vec2 const &p) const
{
	if (!fcppt::math::box::contains_point(
			rect(
				vec2::null(),
				fcppt::math::dim::structure_cast<dim2>(
					static_cast<array::size_type>(cell_size_) * heights_.dimension())),
			p))
		return static_cast<scalar>(0);
	
	triangle const t = 
		enclosing_triangle(
			p);

	plane const vec_plane = 
		math::triangle::to_plane(
			t);

	return
		(vec_plane.lambda() - vec_plane.normal()[0] * p[0] - vec_plane.normal()[2] * p[1]) / vec_plane.normal()[1];
}

insula::height_map::triangle const
insula::height_map::object::enclosing_triangle(
	vec2 const &p) const
{
	// This is too strict due to numerical inaccuracies
	FCPPT_ASSERT_MESSAGE(
		fcppt::math::box::contains_point(
			rect(
				vec2::null(),
				fcppt::math::dim::structure_cast<dim2>(
					static_cast<array::size_type>(cell_size_) * heights_.dimension())),
			p),
		FCPPT_TEXT("The point is not inside the height field"));

	// Determine which grid cell we're talking about
	array::dim const cell = 
		fcppt::math::vector::structure_cast<array::dim>(
			p / cell_size_);

//	fcppt::io::cout << "cell size is " << cell_size << ", point is " << p << "\n";
//	fcppt::io::cout << "calculated the cell: " << cell << "\n";

	// Then, define a line through the top left and the bottom right
	// corner of the quad (the diagonal line) in slope form: 
	// f(x)=m*x+b
	dim2 const 
		topleft = 
			fcppt::math::dim::structure_cast<dim2>(cell) * 
			static_cast<scalar>(cell_size_),
		bottomright = 
			topleft + 
			dim2(
				cell_size_,
				cell_size_);

//	fcppt::io::cout << "cell's top left: " << topleft << ", cell's bottom right: " << bottomright << "\n";

	// This should only happen if cell_size is zero.
	FCPPT_ASSERT(
		!fcppt::math::almost_zero(
			topleft.w() - bottomright.w()));

	scalar const 
		denom = bottomright.w() - topleft.w(),
		m = (bottomright.h() - topleft.h())/denom,
		//b = cross(topleft,bottomright)/denom;
		b = (bottomright.w() * topleft.h() - bottomright.h()*topleft.w())/denom;

//	fcppt::io::cout << "m: " << m << ", b: " << b << "\n";

	if (p.y() > (m * p.x() + b))
		return 
			triangle(
				{
					points_[vec3_array::dim(cell.w(),cell.h())],
					points_[vec3_array::dim(cell.w()+1,cell.h()+1)],
					points_[vec3_array::dim(cell.w()+1,cell.h())] 
				});

	return 
		triangle(
			{
				points_[vec3_array::dim(cell.w(),cell.h())],
				points_[vec3_array::dim(cell.w(),cell.h()+1)],
				points_[vec3_array::dim(cell.w()+1,cell.h()+1)]
			});
}

insula::height_map::array const &
insula::height_map::object::heights() const
{
	return 
		heights_;
}

insula::height_map::array const &
insula::height_map::object::gradient() const
{
	return 
		gradient_;
}

insula::graphics::scalar
insula::height_map::object::cell_size() const
{
	return 
		cell_size_;
}

insula::graphics::scalar
insula::height_map::object::height_scaling() const
{
	return 
		height_scaling_;
}

insula::height_map::object::~object() {}
