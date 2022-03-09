#include <redoom/physics/collisions/CollisionResolver.hh>

#include <redoom/physics/Body.hh>

namespace redoom::physics
{
void CollisionResolver::resolve(CollisionManifold& manifold) const noexcept
{
  manifold.body_a.get().addForce(
      {-1.0f * manifold.body_a.get().getLinearVelocity(),
          Force::Type::Acceleration});
  manifold.body_b.get().addForce(
      {-1.0f * manifold.body_b.get().getLinearVelocity(),
          Force::Type::Acceleration});
}
} // namespace redoom::physics
