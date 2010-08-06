#ifndef INSULA_PHYSICS_TO_BULLET_TYPE_HPP_INCLUDED
#define INSULA_PHYSICS_TO_BULLET_TYPE_HPP_INCLUDED

#include <BulletCollision/CollisionShapes/btConcaveShape.h>

namespace insula
{
namespace physics
{
// This is needed in the height_field shape
template<typename T>
struct to_bullet_type
{
};

template<>
struct to_bullet_type<float>
{
	static PHY_ScalarType const value = PHY_FLOAT;
};

template<>
struct to_bullet_type<double>
{
	static PHY_ScalarType const value = PHY_DOUBLE;
};

template<>
struct to_bullet_type<int>
{
	static PHY_ScalarType const value = PHY_INTEGER;
};

template<>
struct to_bullet_type<short>
{
	static PHY_ScalarType const value = PHY_SHORT;
};

template<>
struct to_bullet_type<unsigned char>
{
	static PHY_ScalarType const value = PHY_UCHAR;
};
}
}

#endif
