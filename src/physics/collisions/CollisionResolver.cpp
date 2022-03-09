#include <redoom/physics/collisions/CollisionResolver.hh>

#include <redoom/physics/Body.hh>

namespace redoom::physics
{
void CollisionResolver::resolve(CollisionManifold& manifold) const noexcept
{
  manifold.body_a.get().addForce(
      {-1.0f * manifold.body_a.get().getLinearVelocity() * 2.0f,
          Force::Type::VelocityChange});
  manifold.body_b.get().addForce(
      {-1.0f * manifold.body_b.get().getLinearVelocity() * 2.0f,
          Force::Type::VelocityChange});
}
} // namespace redoom::physics
