#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <stack>
#include <type_traits>

namespace redoom::memory
{
template <typename T>
class ObjectPool
{
private:
  class Chunk
  {
  public:
    explicit Chunk(std::size_t size) noexcept
      : chunk_size_{size}
      , buffer_(sizeof(T) * size)
    {
      for (auto i = this->chunk_size_; i > 0; --i)
        this->available_.push(i - 1);
    }
    Chunk(Chunk const& b) noexcept = default;
    Chunk(Chunk&& b) noexcept = default;
    ~Chunk() noexcept
    {
      assert(this->available_.size() == this->chunk_size_
             && "Some objects were not released");
    }

    Chunk& operator=(Chunk const& rhs) noexcept = default;
    Chunk& operator=(Chunk&& rhs) noexcept = default;

    [[nodiscard]] bool isFull() const noexcept
    {
      return this->available_.empty();
    }

    template <typename... Args>
    [[nodiscard]] T* get(Args&&... args) noexcept
    {
      static_assert(std::is_constructible_v<T, Args...>,
          "Type T must be constructible with Args.");
      auto const index = this->claimIndex();
      auto* dest = this->getObjectPtr(index);
      return std::construct_at(dest, std::forward<Args>(args)...);
    }

    void release(T* ptr) noexcept
    {
      auto const index = this->getObjectId(ptr);
      auto lock = std::lock_guard{*this->mutex_};
      this->available_.push(index);
    }

    [[nodiscard]] bool contains(T* ptr) const noexcept
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto region_start = reinterpret_cast<std::intptr_t>(this->buffer_.data());
      auto region_size =
          static_cast<std::intptr_t>(sizeof(T) * this->chunk_size_);
      std::intptr_t region_end = region_start + region_size;
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto object_ptr = reinterpret_cast<std::intptr_t>(ptr);
      auto res = (region_start <= object_ptr && object_ptr < region_end);
      return res;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
      return this->chunk_size_;
    }

  private:
    [[nodiscard]] std::size_t claimIndex() noexcept
    {
      auto lock = std::lock_guard{*this->mutex_};
      auto const index = this->available_.top();
      this->available_.pop();
      return index;
    }

    [[nodiscard]] T* getObjectPtr(std::size_t index) noexcept
    {
      // NOLINTNEXTLINE
      return reinterpret_cast<T*>(&this->buffer_[index * sizeof(T)]);
    }

    [[nodiscard]] std::size_t getObjectId(T* ptr) const noexcept
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto iptr = reinterpret_cast<std::intptr_t>(ptr);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto optr = reinterpret_cast<std::intptr_t>(this->buffer_.data());
      auto index = static_cast<unsigned long>(iptr - optr) / sizeof(T);
      return index;
    }

    std::size_t chunk_size_;
    std::vector<std::byte> buffer_;
    std::stack<std::size_t> available_;
    mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  };

public:
  ObjectPool() noexcept = default;
  ObjectPool(ObjectPool const& b) noexcept = default;
  ObjectPool(ObjectPool&& b) noexcept = default;
  ~ObjectPool() noexcept = default;

  ObjectPool& operator=(ObjectPool const& rhs) noexcept = default;
  ObjectPool& operator=(ObjectPool&& rhs) noexcept = default;

  Chunk& grow() noexcept
  {
    auto lock = std::lock_guard{*this->mutex_};
    auto const size =
        this->chunks_.empty() ? 1 : this->chunks_.back().size() * 2;
    return this->chunks_.emplace_back(size);
  }

  template <typename... Args>
  [[nodiscard]] T* get(Args&&... args)
  {
    {
      auto lock = std::lock_guard{*this->mutex_};
      for (auto it = this->chunks_.rbegin(); it != this->chunks_.rend(); ++it)
        if (!it->isFull())
          return it->get(std::forward<Args>(args)...);
    }
    auto& new_chunk = this->grow();
    return new_chunk.get(std::forward<Args>(args)...);
  }

  void release(T* ptr)
  {
    auto type_ptr = static_cast<T*>(ptr);

    // NOTE: This surprisingly proves to perform better than a binary search
    // using std::lower_bound(rbegin, rend, value, std::greater)
    auto lock = std::lock_guard{*this->mutex_};
    for (auto it = this->chunks_.rbegin(); it != this->chunks_.rend(); ++it) {
      if (it->contains(type_ptr)) {
        it->release(type_ptr);
        return;
      }
    }
    assert("Could not release pointer, it does not belong to the ObjectPool"
           == nullptr);
  }

private:
  std::list<Chunk> chunks_;
  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
};
} // namespace redoom::memory
