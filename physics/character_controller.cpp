#include "character_controller.hpp"
#include "transform_to_gizmo.hpp"
#include "vec3_to_bullet.hpp"
#include "world.hpp"
#include "bullet_to_vec3.hpp"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <fcppt/text.hpp>
#include <fcppt/io/clog.hpp>
#include <boost/foreach.hpp>
#include <boost/range/iterator_range_core.hpp>

namespace
{
/*
 * Returns the reflection direction of a ray going 'direction' hitting a surface with normal 'normal'
 *
 * from: http://www-cs-students.stanford.edu/~adityagp/final/node3.html
 */
btVector3 
computeReflectionDirection(
	btVector3 const &direction, 
	btVector3 const &normal)
{
	return direction - (btScalar(2.0) * direction.dot(normal)) * normal;
}

/*
 * Returns the portion of 'direction' that is parallel to 'normal'
 */
btVector3 const
parallelComponent(
	btVector3 const &direction, 
	btVector3 const & normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
}

/*
 * Returns the portion of 'direction' that is perpindicular to 'normal'
 */
btVector3 
perpindicularComponent(
	btVector3 const &direction, 
	btVector3 const &normal)
{
	return direction - parallelComponent(direction, normal);
}

class closest_result
: 
	public btCollisionWorld::ClosestRayResultCallback
{
public:
	explicit
	closest_result(
		btCollisionObject const * const _me) 
	: 
		btCollisionWorld::ClosestRayResultCallback(
			btVector3(0.0, 0.0, 0.0), 
			btVector3(0.0, 0.0, 0.0)),
		me_(_me)
	{
	}

	btScalar 
	addSingleResult(
		btCollisionWorld::LocalRayResult &rayResult,
		bool const normalInWorldSpace)
	{
		if (rayResult.m_collisionObject == me_)
			return 1.0;

		return 
			ClosestRayResultCallback::addSingleResult(
				rayResult, 
				normalInWorldSpace);
	}

	btCollisionObject const * const me_;
};

class closest_convex_result 
: 
	public btCollisionWorld::ClosestConvexResultCallback
{
public:
	explicit
	closest_convex_result(
		btCollisionObject const * const _me) 
	: 
		btCollisionWorld::ClosestConvexResultCallback(
			btVector3(0.0, 0.0, 0.0), 
			btVector3(0.0, 0.0, 0.0)),
		me_(_me)
	{
	}

	btScalar 
	addSingleResult(
		btCollisionWorld::LocalConvexResult &convexResult,
		bool const normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me_)
			return 1.0;

		return 
			ClosestConvexResultCallback::addSingleResult(
				convexResult, 
				normalInWorldSpace);
	}

	btCollisionObject const * const me_;
};
}

insula::physics::character_controller::character_controller(
	world &_world,
	scalar width,
	scalar height,
	vec3 const &position)
:
	physics::object(
		object_type::character),
	world_(
		_world),
	shape_(
		width,
		height/2),
	ghost_object_(),
	walk_vector_(
		0,0,0),
	normalized_walk_vector_(
		0,0,0),
	step_height_(
		static_cast<btScalar>(
			0.3))
{
	// Ghost init. begin
	ghost_object_.setUserPointer(
		this);
	ghost_object_.setWorldTransform(
		btTransform(
			btMatrix3x3::getIdentity(),
			// NOTE: We "correct" the player's position manually here. I
			// don't know if this is better done outside this class
			vec3_to_bullet(
				position + 
				physics::vec3(0,height/2,0))));
	ghost_object_.setCollisionShape(
		&shape_);
	// I don't know what this is for
	ghost_object_.setCollisionFlags(
		btCollisionObject::CF_CHARACTER_OBJECT);
	world_.handle().getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(
		new btGhostPairCallback());
	// Ghost init. end

	world_.handle().addCollisionObject(
		&ghost_object_,
		btBroadphaseProxy::CharacterFilter, 
		btBroadphaseProxy::StaticFilter
			| btBroadphaseProxy::DefaultFilter);
	world_.handle().addAction(
		this);
}

insula::physics::gizmo const
insula::physics::character_controller::gizmo() const
{
	return 
		transform_to_gizmo(
			ghost_object_.getWorldTransform());
}

void
insula::physics::character_controller::update(
	time_delta const delta)
{
	/*
	impl_->setWalkDirection(
		vec3_to_bullet(
			walk_vector_  * delta));
	*/
}

void
insula::physics::character_controller::walk_vector(
	vec3 const &_walk_vector)
{
	walk_vector_ = 
		vec3_to_bullet(
			_walk_vector);

	normalized_walk_vector_ = 
		_walk_vector != vec3::null()
		?
			walk_vector_.normalized()
		:
			btVector3(0,0,0);
}

void
insula::physics::character_controller::updateAction(
	btCollisionWorld * const world,
	btScalar const delta)
{
	touching_contact_ = false;

	unsigned const max_penetration_loops = 4;
	unsigned loops_done;

	for(
		loops_done = 0; 
		recover_from_penetration(
			*world) 
			&& loops_done < max_penetration_loops; 
		++loops_done)
		touching_contact_ = true;

	if (loops_done == max_penetration_loops)
	{
		static unsigned counter = 0;
		fcppt::io::clog 
			<< counter++ << FCPPT_TEXT(": Couldn't recover the character from penetration (gnihihi)\n");
	}

	current_position_ = target_position_ = 
		ghost_object_.getWorldTransform().getOrigin();
	
	// We store the transformation _before_ the step functions, then
	// we _apply_ the step functions and just set the _position_, not
	// the _matrix_ to the new position
	btTransform saved_transform = 
		ghost_object_.getWorldTransform();

	// fills current position and current step height
	step_up(
		*world);
	step_forward(
		*world,
		delta);
	step_down(
		*world);

	saved_transform.setOrigin(
		current_position_);
	ghost_object_.setWorldTransform(
		saved_transform);
}

void
insula::physics::character_controller::debugDraw(
	btIDebugDraw *)
{
}

insula::physics::character_controller::~character_controller()
{
	world_.handle().removeCollisionObject(
		&ghost_object_);
	world_.handle().removeAction(
		this);
}

// In this function, we first test which objects _potentially_ overlap
// with the ghost object. Then we see if they overlap indeed. Then we
// traverse all the collision points and move the body along the
// contact normal to "undo" the collision.
bool
insula::physics::character_controller::recover_from_penetration(
	btCollisionWorld &world)
{
	bool penetration = false;

	world.getDispatcher()->dispatchAllCollisionPairs(
		ghost_object_.getOverlappingPairCache(), 
		world.getDispatchInfo(), 
		world.getDispatcher());

	btVector3 current_position = 
		ghost_object_.getWorldTransform().getOrigin();
	
	// This stores the minimum (!) distance stored in the manifolds. We
	// want the "closest" object to update the contact normal.
	btScalar maxPen = 0;

	// This loop runs over every object which intersects with the ghost
	// object (the "overlapping pairs") and fills the manifolds array
	BOOST_FOREACH(
		btBroadphasePair &collisionPair,
		boost::make_iterator_range(
			&ghost_object_.getOverlappingPairCache()->getOverlappingPairArray()[0],
			&ghost_object_.getOverlappingPairCache()->getOverlappingPairArray()[0] + ghost_object_.getOverlappingPairCache()->getOverlappingPairArray().size()))
	{
		btManifoldArray	manifolds;
		manifolds.resize(0);

		// Use the shape-specific collision algorithm to construct
		// collision points?
		if (collisionPair.m_algorithm)
			collisionPair.m_algorithm->getAllContactManifolds(
				manifolds);
		
		BOOST_FOREACH(
			btPersistentManifold *manifold,
			boost::make_iterator_range(
				&manifolds[0],
				&manifolds[0] + manifolds.size()))
		{
			// I think this determines which body is body 0 and which is
			// 1. The normal has to be inverted accordingly
			btScalar const directionSign = 
				manifold->getBody0() == &ghost_object_ 
				? static_cast<btScalar>(-1) 
				: static_cast<btScalar>(1);

			BOOST_FOREACH(
				btManifoldPoint const &pt,
				boost::make_iterator_range(
					&manifold->getContactPoint(0),
					&manifold->getContactPoint(0) + manifold->getNumContacts()))
			{
				if (pt.getDistance() < static_cast<btScalar>(0))
				{
					if (pt.getDistance() < maxPen)
					{
						maxPen = pt.getDistance();
						touching_normal_ = 
							pt.m_normalWorldOnB * directionSign;
					}
					// What's the 0.2 doing here?
					current_position += 
						pt.m_normalWorldOnB 
							* directionSign 
							* pt.getDistance() 
							* static_cast<btScalar>(0.2);
					penetration = true;
				} 
			}
		}
	}

	btTransform newTrans = 
		ghost_object_.getWorldTransform();
	newTrans.setOrigin(
		current_position);
	ghost_object_.setWorldTransform(
		newTrans);
	return penetration;
}

void
insula::physics::character_controller::step_up(
	btCollisionWorld &)
{
	target_position_ = 
		current_position_ 
			+ btVector3(0,1,0) * step_height_;

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	// FIXME: Handle penetration properly 
	// We start from the current position, but a bit higher (magic
	// number 0.1)
	start.setOrigin(
		current_position_ 
			+ btVector3(0,1,0) * static_cast<btScalar>(0.1));
	end.setOrigin(
		target_position_);

	closest_convex_result callback(
		&ghost_object_);
	callback.m_collisionFilterGroup = 
		ghost_object_.getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = 
		ghost_object_.getBroadphaseHandle()->m_collisionFilterMask;
	
	ghost_object_.convexSweepTest(
		&shape_, 
		start, 
		end, 
		callback, 
		world_.handle().getDispatchInfo().m_allowedCcdPenetration); 
	
	if (callback.hasHit())
	{
		// we moved up only a fraction of the step height
		current_step_offset_ = 
			step_height_ 
				* callback.m_closestHitFraction;
		current_position_.setInterpolate3(
			current_position_, 
			target_position_,
			callback.m_closestHitFraction);
	} 
	else 
	{
		current_step_offset_ = step_height_;
		current_position_ = target_position_;
	}
}

void
insula::physics::character_controller::step_forward(
	btCollisionWorld &world,
	btScalar const delta)
{
	scalar const added_margin = 
		static_cast<scalar>(0.02);

	// phase 2: forward and strafe
	btTransform start, end;
	target_position_ = 
		current_position_ + 
		walk_vector_ * delta;
	start.setIdentity();
	end.setIdentity();
	
	btScalar 
		fraction = 1,
		distance2 = (current_position_-target_position_).length2();

	if (touching_contact_)
		if (normalized_walk_vector_.dot(touching_normal_) > static_cast<btScalar>(0))
			update_target_position(
				touching_normal_);

	// This is the usual loop where we try to move until we are held
	// back by a collision
	for (unsigned i = 0; i < 10 && fraction > static_cast<btScalar>(0); ++i)
	{
		start.setOrigin(
			current_position_);
		end.setOrigin(
			target_position_);

		closest_convex_result callback(&ghost_object_);
		callback.m_collisionFilterGroup = 
			ghost_object_.getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = 
			ghost_object_.getBroadphaseHandle()->m_collisionFilterMask;

		// We temporarily increase the margin for the sweep test.
		btScalar const margin = 
			shape_.getMargin();
		shape_.setMargin(
			margin + added_margin);

		ghost_object_.convexSweepTest(
			&shape_, 
			start, 
			end, 
			callback, 
			world.getDispatchInfo().m_allowedCcdPenetration);
		
		shape_.setMargin(
			margin);

		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
		{	
			// we moved only a fraction
			btScalar const hitDistance = 
				(callback.m_hitPointWorld - current_position_).length();

			// If the distance is farther than the collision margin, move. I
			// think this reduces "jitter" when walking next to a wall, for
			// example
			if (hitDistance > added_margin)
			{
				current_position_.setInterpolate3(
					current_position_, 
					target_position_, 
					callback.m_closestHitFraction);
			}

			update_target_position(
				callback.m_hitNormalWorld);
			btVector3 currentDir = target_position_ - current_position_;
			distance2 = currentDir.length2();
			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();
				// See Quake2: "If velocity is against original velocity, stop
				// ead to avoid tiny oscilations in sloping corners."
				if (currentDir.dot(normalized_walk_vector_) <= static_cast<btScalar>(0))
					break;
			}
		} 
		else 
		{
			// we moved whole way
			current_position_ = target_position_;
		}
	}
}

void
insula::physics::character_controller::step_down(
	btCollisionWorld &world)
{
	// phase 3: down
	btVector3 const 
		step_drop = 
			btVector3(0,1,0) * current_step_offset_,
		gravity_drop = 
			btVector3(0,1,0) * step_height_; 

	target_position_ -= (step_drop + gravity_drop);

	// FIXME: ugly
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(
		current_position_);
	end.setOrigin(
		target_position_);

	closest_convex_result callback(
		&ghost_object_);
	callback.m_collisionFilterGroup = 
		ghost_object_.getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = 
		ghost_object_.getBroadphaseHandle()->m_collisionFilterMask;
	
	ghost_object_.convexSweepTest(
		&shape_, 
		start, 
		end, 
		callback, 
		world.getDispatchInfo().m_allowedCcdPenetration);

	if (callback.hasHit())
	{
		// we dropped a fraction of the height -> hit floor
		current_position_.setInterpolate3(
			current_position_, 
			target_position_, 
			callback.m_closestHitFraction);
	} 
	else 
	{
		// we dropped the full height
		current_position_ = target_position_;
	}
}

void
insula::physics::character_controller::update_target_position(
	btVector3 const &hitNormal)
{
	btVector3 movementDirection = target_position_ - current_position_;
	btScalar movementLength = movementDirection.length();

	if (movementLength <= SIMD_EPSILON)
		return;

	movementDirection.normalize();

	btVector3 reflectDir = 
		computeReflectionDirection(
			movementDirection, 
			hitNormal);
	reflectDir.normalize();

	btVector3 parallelDir, perpindicularDir;

	parallelDir = 
		parallelComponent(
			reflectDir, 
			hitNormal);
	perpindicularDir = 
		perpindicularComponent(
			reflectDir, 
			hitNormal);

	btVector3 perpComponent = 
		perpindicularDir * movementLength;
	target_position_ += perpComponent;
}
