#include <redoom/physics/Body.hh>

namespace redoom::physics
{
Body::Body(World& pworld, Body::Definition def) noexcept
  : world{pworld}
  , type{def.type}
  , position{def.position}
  , angle{def.angle}
  , linear_velocity{def.linear_velocity}
  , angular_velocity{def.angular_velocity}
  , has_fixed_rotation{def.has_fixed_rotation}
  , gravity_scale{def.gravity_scale}
{
}
} // namespace redoom::physics
