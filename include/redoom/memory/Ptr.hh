#pragma once

#include <cassert>
#include <functional>

namespace redoom::memory
{
template <typename T, typename Deleter>
class Ptr
{
public:
  Ptr(T* pptr, Deleter pdeleter) noexcept
    : ptr{pptr}
    , deleter{pdeleter}
  {
  }
  ~Ptr() noexcept
  {
    this->release();
  }
  Ptr(Ptr const& rhs) noexcept = delete;
  Ptr(Ptr&& rhs) noexcept
    : ptr{std::move(rhs.ptr)}
    , deleter{std::move(rhs.deleter)}
  {
    rhs.ptr = nullptr;
  }

  Ptr& operator=(Ptr const& rhs) noexcept = delete;
  Ptr& operator=(Ptr&& rhs) noexcept
  {
    this->release();
    this->ptr = std::move(rhs.ptr);
    rhs.ptr = nullptr;
    this->deleter = std::move(rhs.deleter);
    return *this;
  }
  bool operator==(Ptr const& rhs) const noexcept
  {
    return this->ptr == rhs.ptr;
  }
  bool operator!=(Ptr const& rhs) const noexcept
  {
    return !(*this == rhs);
  }
  T& operator*() noexcept
  {
    return *this->ptr;
  }
  T* operator->() noexcept
  {
    return this->ptr;
  }
  T const& operator*() const noexcept
  {
    return *this->ptr;
  }
  T const* operator->() const noexcept
  {
    return this->ptr;
  }
  template <typename U>
  operator Ptr<U, Deleter>() noexcept // NOLINT
  {
    auto res = Ptr<U, Deleter>{static_cast<U*>(this->ptr), this->deleter};
    this->ptr = nullptr;
    return res;
  }

  T* get() const noexcept
  {
    return this->ptr;
  }

private:
  void release()
  {
    if (this->ptr == nullptr)
      return;
    std::invoke(this->deleter, this->ptr);
  }

  T* ptr;
  Deleter deleter;
};
} // namespace redoom::memory
