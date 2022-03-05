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
    this->items_.emplace_back(std::ref(item));
  }

  void remove(Body const& item) noexcept override
  {
    auto it = std::find_if(this->items_.begin(),
        this->items_.end(),
        [&item](std::reference_wrapper<Body> const& lhs) {
          return std::addressof(lhs.get()) == std::addressof(item);
        });
    if (it == this->items_.end())
      return;
    this->items_.erase(it);
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

    if (this->items_.size() <= 1)
      return manifolds;

    auto max_size = 0u;
    for (auto i = 1u; i < this->items_.size(); ++i)
      max_size += this->items_.size() - i;
    manifolds.reserve(max_size);

    for (auto i = 0u; i < this->items_.size() - 1; ++i) {
      for (auto j = i + 1; j < this->items_.size(); ++j) {
        manifolds.emplace_back(
            CollisionManifold{this->items_[i], this->items_[j], {}});
      }
    }
    return manifolds;
  }

private:
  std::vector<std::reference_wrapper<Body>> items_;
};
}; // namespace redoom::physics
