#include "sphere.hpp"
#include "world.hpp"
#include "transform_from_vec3.hpp"
#include "bullet_to_vec3.hpp"
#include "bullet_to_mat4.hpp"
#include "../graphics/camera.hpp"
#include "../media_path.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include "vf/vertex_view.hpp"
#include "../skydome/sphere_point.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
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
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>

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
	std::size_t
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
			// Remember the beginning index of the current circle so we can wrap around later in the inner loop
			value_type const beginning = 
				current;
			// The last index would wrap around the first one, so it_lon-1
			for (size_type lon = 0; lon < static_cast<size_type>(it_lon); ++lon)
			{
				value_type const 
					next = 
						lon == static_cast<size_type>(it_lon-1)
						?
							static_cast<value_type>(beginning)
						:
							static_cast<value_type>(
								current+1),
					below = 
						static_cast<value_type>(
							current + it_lon),
					next_below = 
						lon == static_cast<size_type>(it_lon-1)
						?
							static_cast<value_type>(
								beginning + it_lon)
						:
							static_cast<value_type>(
								below+1);
				
				*it++ = current;
				*it++ = next_below;
				*it++ = below;

				*it++ = current;
				*it++ = next_below;
				*it++ = next;
				
				indices += 6;
			
				++current;
			}
		}
		
		FCPPT_ASSERT(indices <= ib_size);
	}
};
}

insula::physics::sphere::sphere(
	sge::renderer::device_ptr const _renderer,
	graphics::camera &_camera,
	world &_world,
	scalar const radius,
	scalar const mass,
	vec3 const &position)
:
	world_(
		_world),
	camera_(
		_camera),
	transform_(
		transform_from_vec3(
			position)),
	renderer_(
		_renderer),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("physics_vertex.glsl"),
		media_path()/FCPPT_TEXT("physics_fragment.glsl"))
{
	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	sge::renderer::size_type lats = 10,lons = 10;

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				// The tops
				1 + 
				// The inner rings minus the two tops
				(lats - 1) * lons),
			sge::renderer::resource_flags::none);

	ib_ = 
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// For each pair of rings we have "it_lon" quads. We have
			// "it_lat-1" rings and "it_lat-2" pairs of rings.  This makes for
			// "it_lat-2" quads, each having 2 triangles and each triangle has
			// 3 indices.
			static_cast<sge::renderer::size_type>(
				// the quads
				(lats-2) * lons * 2 * 3 +
				// the triangles
				lons * 3), 
			sge::renderer::resource_flags::none);

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	// The top
	vb_it->set<vf::position>(
		skydome::sphere_point(
			radius,
			0,
			0));
	
	vb_it++;

	vf::vertex_view::size_type vertex_count = 
		1;

	scalar const 
		increment_lat = 
			//halfpi / static_cast<scalar>(lats),
			fcppt::math::pi<scalar>() / static_cast<scalar>(lats),
		increment_lon = 
			fcppt::math::twopi<scalar>()/
			static_cast<scalar>(lons);

	scalar lat = increment_lat;
	// We make one less iteration because we don't want the top
	// NOTE: We use size_type here FOR A REASON! floating point loops are inaccurate
	for (sge::renderer::size_type i = 1; i < lats; ++i)
	{
		scalar lon = 
			static_cast<scalar>(0);
		for (sge::renderer::size_type j = 0; j < lons; ++j)
		{
			++vertex_count;

			vb_it->set<vf::position>(
				skydome::sphere_point(
					radius,
					lat,
					lon));

			++vb_it;
			lon += increment_lon;
		}
		lat += increment_lat;
	}
	
	FCPPT_ASSERT(vertex_count <= vb_->size());

	fcppt::variant::apply_unary(
		index_visitor(
			static_cast<index_visitor::size_type>(
				ib_->size()),
			lats,
			lons),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());

	shape_.reset(
		new btSphereShape(
			radius));

	btVector3 local_inertia;
	shape_->calculateLocalInertia(
		mass,
		local_inertia);

	body_.reset(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				mass,
				// Motion state
				this,
				shape_.get(),
				local_inertia)));

	body_->setLinearFactor(
		btVector3(1,1,1));
	
	world_.add(
		*body_.get());

}

void
insula::physics::sphere::render()
{
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);
	
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.perspective() * 
		camera_.rotation() * 
		camera_.translation() * 
		mat_transformed_);

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::off));

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

insula::physics::sphere::~sphere()
{
	world_.remove(
		*body_.get());
}

void
insula::physics::sphere::getWorldTransform(
	btTransform &t) const
{
	t = transform_;
}

void
insula::physics::sphere::setWorldTransform(
	btTransform const &t)
{
	transform_ = t;
	mat_transformed_ = 
		
		// NOTE: The order here is important. It has to be like this.
		(fcppt::math::matrix::translation(
			bullet_to_vec3(
				t.getOrigin()))) * 
		bullet_to_mat4(
			t.getBasis());
}
