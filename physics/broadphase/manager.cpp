#include "manager.hpp"
#include "culler.hpp"
#include "intrusive_wrapper.hpp"
#include "../../graphics/camera/object.hpp"
#include "../../graphics/frustum.hpp"
#include "../../math/extract_frustum.hpp"
#include "../vec3_to_bullet.hpp"
#include <boost/foreach.hpp>
#include <cstddef>
#include <array>

insula::physics::broadphase::manager::manager(
	graphics::camera::object const &_camera)
:
	camera_(
		_camera),
	// This is not zero so it's different from the default iteration
	// value of the objects (which is 0)
	current_iteration_(
		1)
{
}

insula::physics::broadphase::shared_object_ptr const
insula::physics::broadphase::manager::create()
{
	shared_object_ptr temp(
		new intrusive_wrapper());

	objects_.push_back(
		*temp);

	return temp;
}

void
insula::physics::broadphase::manager::update()
{
	std::size_t const plane_count = 4;
	
	std::array<btVector3,plane_count> normals;
	std::array<scalar,plane_count> lambdas;

	graphics::frustum const f = 
		math::extract_frustum<graphics::scalar>(
			camera_.mvp());

	normals[0] = vec3_to_bullet(f.left().normal());
	normals[1] = vec3_to_bullet(f.right().normal());
	normals[2] = vec3_to_bullet(f.top().normal());
	normals[3] = vec3_to_bullet(f.bottom().normal());

	lambdas[0] = f.left().lambda();
	lambdas[1] = f.right().lambda();
	lambdas[2] = f.top().lambda();
	lambdas[3] = f.bottom().lambda();

	current_iteration_++;

	BOOST_FOREACH(
		object_list::reference r,
		objects_)
	{
		culler culler_(
			current_iteration_);
		//fcppt::io::cout << "collide static\n";
		btDbvt::collideKDOP(
			r.m_sets[0].m_root,
			normals.data(),
			lambdas.data(),
			static_cast<int>(
				normals.size()),
			culler_);

		//fcppt::io::cout << "collide dynamic\n";
		btDbvt::collideKDOP(
			r.m_sets[1].m_root,
			normals.data(),
			lambdas.data(),
			static_cast<int>(
				normals.size()),
			culler_);
	}
}

insula::physics::broadphase::iteration
insula::physics::broadphase::manager::current_iteration() const
{
	return current_iteration_;
}
