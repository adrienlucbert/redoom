#pragma once

#include <redoom/physics/collisions/CollisionFilter.hh>

#include <iostream>

namespace redoom::physics
{
class PairFilter : public CollisionFilter
{
public:
  ~PairFilter() noexcept override = default;

  void insert(Body& item) noexcept override
  {
    this->items.emplace_back(std::ref(item));
  }

  void remove(Body const& item) noexcept override
  {
    auto it = std::find_if(this->items.begin(),
        this->items.end(),
        [&item](std::reference_wrapper<Body> const& lhs) {
          return std::addressof(lhs.get()) == std::addressof(item);
        });
    if (it == this->items.end())
      return;
    this->items.erase(it);
  }

  void update(
      std::vector<std::reference_wrapper<Body>> /*moved_items*/) noexcept
      override
  {
  }

  [[nodiscard]] std::vector<CollisionManifold> getPossibleCollisions() noexcept
      override
  {
    auto manifolds = std::vector<CollisionManifold>{};

    if (this->items.size() <= 1)
      return manifolds;

    auto max_size = 0u;
    for (auto i = 1u; i < this->items.size(); ++i)
      max_size += this->items.size() - i;
    manifolds.reserve(max_size);

    for (auto i = 0u; i < this->items.size() - 1; ++i) {
      for (auto j = i + 1; j < this->items.size(); ++j) {
        manifolds.emplace_back(
            CollisionManifold{this->items[i], this->items[j], {}});
      }
    }
    return manifolds;
  }

private:
  std::vector<std::reference_wrapper<Body>> items;
};
}; // namespace redoom::physics
