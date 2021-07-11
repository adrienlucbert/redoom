#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <list>
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
      : chunk_size{size}
      , buffer(sizeof(T) * size)
    {
      for (auto i = this->chunk_size; i > 0; --i)
        this->available.push(i - 1);
    }
    Chunk(Chunk const& b) noexcept = default;
    Chunk(Chunk&& b) noexcept = default;
    ~Chunk() noexcept
    {
      assert(this->available.size() == this->chunk_size
             && "Some objects were not released");
    }

    Chunk& operator=(Chunk const& rhs) noexcept = default;
    Chunk& operator=(Chunk&& rhs) noexcept = default;

    [[nodiscard]] bool isFull() const noexcept
    {
      return this->available.empty();
    }

    template <typename... Args>
    [[nodiscard]] T* get(Args&&... args) noexcept
    {
      static_assert(std::is_constructible_v<T, Args...>,
          "Type T must be constructible with Args.");
      auto const index = this->claimIndex();
      auto* dest = this->getObjectPtr(index);
      auto src = T{std::forward<Args>(args)...};
      std::memcpy(
          static_cast<void*>(dest), static_cast<void*>(&src), sizeof(T));
      return dest;
    }

    void release(T* ptr) noexcept
    {
      auto index = this->getObjectId(ptr);
      this->available.push(index);
    }

    [[nodiscard]] bool contains(T* ptr) const noexcept
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto region_start = reinterpret_cast<std::intptr_t>(this->buffer.data());
      auto region_size =
          static_cast<std::intptr_t>(sizeof(T) * this->chunk_size);
      std::intptr_t region_end = region_start + region_size;
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto object_ptr = reinterpret_cast<std::intptr_t>(ptr);
      auto res = (region_start <= object_ptr && object_ptr < region_end);
      return res;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
      return this->chunk_size;
    }

  private:
    [[nodiscard]] std::size_t claimIndex() noexcept
    {
      auto const index = this->available.top();
      this->available.pop();
      return index;
    }

    [[nodiscard]] T* getObjectPtr(std::size_t index) noexcept
    {
      // NOLINTNEXTLINE
      return reinterpret_cast<T*>(&this->buffer[index * sizeof(T)]);
    }

    [[nodiscard]] std::size_t getObjectId(T* ptr) const noexcept
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto iptr = reinterpret_cast<std::intptr_t>(ptr);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      auto optr = reinterpret_cast<std::intptr_t>(this->buffer.data());
      auto index = static_cast<unsigned long>(iptr - optr) / sizeof(T);
      return index;
    }

    std::size_t chunk_size;
    std::vector<std::byte> buffer;
    std::stack<std::size_t> available;
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
    auto const size = this->chunks.empty() ? 1 : this->chunks.back().size() * 2;
    return this->chunks.emplace_back(size);
  }

  template <typename... Args>
  [[nodiscard]] T* get(Args&&... args)
  {
    for (auto it = this->chunks.rbegin(); it != this->chunks.rend(); ++it)
      if (!it->isFull())
        return it->get(std::forward<Args>(args)...);
    auto& new_chunk = this->grow();
    return new_chunk.get(std::forward<Args>(args)...);
  }

  void release(T* ptr)
  {
    auto type_ptr = static_cast<T*>(ptr);

    // NOTE: This surprisingly proves to perform better than a binary search
    // using std::lower_bound(rbegin, rend, value, std::greater)
    for (auto it = this->chunks.rbegin(); it != this->chunks.rend(); ++it) {
      if (it->contains(type_ptr)) {
        it->release(type_ptr);
        return;
      }
    }
  }

private:
  std::list<Chunk> chunks;
};
} // namespace redoom::memory