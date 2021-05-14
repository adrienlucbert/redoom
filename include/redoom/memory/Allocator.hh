#pragma once

template <typename T>
class Allocator
{
public:
  Allocator() noexcept = default;
  Allocator(Allocator const& b) noexcept = default;
  Allocator(Allocator&& b) noexcept = default;
  ~Allocator() noexcept = default;

  Allocator& operator=(Allocator const& rhs) noexcept = default;
  Allocator& operator=(Allocator&& rhs) noexcept = default;

private:
};
