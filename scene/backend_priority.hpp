#ifndef INSULA_SCENE_BACKEND_PRIORITY_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_PRIORITY_HPP_INCLUDED

namespace insula
{
namespace scene
{
namespace backend_priority
{
/// Sometimes you have to draw something before something else. The
/// skydome has to be drawn before all the models because it needs a
/// disable z buffer. So it gets high priority, whereas everything
/// else gets normal priority. I haven't got a use case for something
/// else (low priority for example) yet.
/// Note that higher priorities have lower numbers in this enum, see
/// backend_comparator for an explanation
enum type
{
	high,
	normal
};
}
}
}

#endif
