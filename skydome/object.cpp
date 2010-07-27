#include "sphere_point.hpp"
#include "object.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include "vf/vertex_view.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera.hpp"
#include "../media_path.hpp"
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
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/indexed_primitive_type.hpp>
#include <sge/renderer/primitive_count.hpp>
#include <sge/renderer/first_index.hpp>
#include <sge/renderer/index/i32.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/image/file.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <boost/foreach.hpp>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/math/deg_to_rad.hpp>

// DEBUG
#include <fcppt/io/cout.hpp>
#include <fcppt/math/vector/output.hpp>

namespace
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	typedef 
	insula::skydome::size_type
	size_type;

	size_type ib_size;
	size_type it_lat,it_lon;

	explicit
	index_visitor(
		size_type const ib_size,
		size_type const it_lat,
		size_type const it_lon)
	:
		ib_size(
			ib_size),
		it_lat(
			it_lat),
		it_lon(
			it_lon)
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

		size_type indices = 0;

		// First, the triangles at the top
		for (size_type lons = 1; lons <= it_lon; ++lons)
		{
			/* DEBUG
			fcppt::io::cout
				<< "Adding triangle: "
				<< 0
				<< ","
				<< ((lons == it_lon) ? 1 : (lons+1))
				<< ","
				<< lons
				<< "\n";*/
			*it++ = 0;
			*it++ = 
				static_cast<value_type>(
					lons == it_lon ? 1 : (lons+1));
			*it++ = 
				static_cast<value_type>(
					lons);
			indices += 3;
		}

		// Start at the first vertex after the top
		value_type current = 1;
		// Again, we have it_lat-2 ring pairs (see below)
		for (size_type lats = 0; lats < static_cast<size_type>(it_lat-2); ++lats)
		{
			// The last index would wrap around the first one, so it_lon-1
			for (size_type lon = 0; lon < static_cast<size_type>(it_lon-1); ++lon)
			{
				value_type const below = 
					static_cast<value_type>(
						current + it_lon);
				
				*it++ = current;
				*it++ = static_cast<value_type>(below+1);
				*it++ = static_cast<value_type>(below);

				*it++ = static_cast<value_type>(current+1);
				*it++ = static_cast<value_type>(below+1);
				*it++ = static_cast<value_type>(below);

				indices += 6;
			
				++current;
			}
		}
		
		FCPPT_ASSERT(indices < ib_size);
	}
};
}

insula::skydome::object::object(
	graphics::camera const &_camera,
	sge::renderer::device_ptr const _renderer,
	sge::console::object &obj,
	graphics::scalar const angle,
	size_type const iterations_lat,
	size_type const iterations_long)
:
	camera_(
		_camera),
	renderer_(
		_renderer),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("skydome_vertex.glsl"),
		media_path()/FCPPT_TEXT("skydome_fragment.glsl")),
	shader_to_console_(
		FCPPT_TEXT("skydome"),
		shader_,
		obj)
{
	regenerate_buffer(
		angle,
		iterations_lat,
		iterations_long);

	fcppt::io::cout << "rotation is: " << camera_.rotation() << "\n";
}

void
insula::skydome::object::render()
{
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);
	
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	//fcppt::io::cout << "rotation matrix: " << camera_.rotation() << ", translation matrix: " << camera_.translation() << "\n";

	using fcppt::math::matrix::transpose;

	shader_.set_uniform(
		FCPPT_TEXT("translation"),
		camera_.translation());

	shader_.set_uniform(
		FCPPT_TEXT("rotation"),
		camera_.rotation());

	shader_.set_uniform(
		FCPPT_TEXT("perspective"),
		camera_.perspective());

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::enable_lighting = false)
		 	(sge::renderer::state::cull_mode::off)
		 	(sge::renderer::state::depth_func::off));

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::point);
	/*
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
*/
}

void
insula::skydome::object::regenerate_buffer(
	graphics::scalar,
	size_type const lats,
	size_type const lons)
{
	using graphics::scalar; 

	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				// The top
				1 + 
				// The inner rings minus the two tops
				(lats - 1) * lons),
			sge::renderer::resource_flags::none);

	FCPPT_ASSERT_MESSAGE(
		vb_->size() < 
		static_cast<sge::renderer::size_type>(
			std::numeric_limits<sge::renderer::index::i32>::max()),
		FCPPT_TEXT("The skydome is too big for a 32 bit index!"));

	/*
	ib_ = 
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// For each pair of rings we have "it_lon" quads. We have
			// "it_lat-1" rings and "it_lat-2" pairs of rings.  This makes for
			// "it_lat-2" quads, each having 2 triangles and each triangle has
			// 3 indices.
			static_cast<sge::renderer::size_type>(
				// the quads
				(it_lat-2) * it_lon * 2 * 3 +
				// the triangles
				it_lon * 3), 
			sge::renderer::resource_flags::none);
*/

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	scalar const radius = 
		static_cast<scalar>(
			1);

	scalar const halfpi = 
		fcppt::math::pi<scalar>()/
		static_cast<scalar>(2);

	fcppt::io::cout << "The top has coordinates: " << 
		sphere_point(
			radius,
			0,
			0) << "\n";

	// The top
	vb_it->set<vf::position>(
		sphere_point(
			radius,
			0,
			0));
	
	vb_it++;

	vf::vertex_view::size_type vertex_count = 
		1;

	scalar const 
		increment_lat = 
			halfpi / static_cast<scalar>(lats),
		increment_lon = 
			fcppt::math::twopi<scalar>()/
			static_cast<scalar>(lons);

	scalar lat = increment_lat;
	// We make one less iteration because we don't want the top
	// NOTE: We use size_type here FOR A REASON! floating point loops are inaccurate
	for (size_type i = 1; i < lats; ++i)
	{
		//fcppt::io::cout << "current latitude: " << fcppt::math::rad_to_deg(lat) << "\n";
		scalar lon = 
			static_cast<scalar>(0);
		for (size_type j = 0; j < lons; ++j)
		{
			//fcppt::io::cout << "current longitude: " << fcppt::math::rad_to_deg(lon) << "\n";
			++vertex_count;

			// TODO: Check if this is right!
			vb_it->set<vf::position>(
				sphere_point(
					radius,
					lat,
					lon));

			++vb_it;
			lon += increment_lon;
		}
		lat += increment_lat;
	}
	
	fcppt::io::cout << "there are " << vertex_count << " vertices, the buffer size is " << vb_->size() << "\n";

	FCPPT_ASSERT(vertex_count <= vb_->size());

	/*
	fcppt::variant::apply_unary(
		index_visitor(
			static_cast<index_visitor::size_type>(
				ib_->size()),
			it_lat,
			it_lon),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());*/
}
