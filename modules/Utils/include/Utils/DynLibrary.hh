#pragma once

#include <dlfcn.h>
#include <iostream>
#include <memory>
#include <string_view>

#include <Utils/Expected.hh>

namespace redoom::Utils
{
class DLLoader;

class DynLibrary
{
public:
  template <typename SymT>
  Expected<SymT> getSymbol(std::string_view symbol) const noexcept
  {
    auto* sym = dlsym(this->handle_.get(), symbol.data());
    if (sym == nullptr)
      return make_formatted_unexpected("{}: symbol not found", symbol);
    return (reinterpret_cast<SymT>(sym)); // NOLINT
  }

private:
  explicit DynLibrary(void* handle) noexcept
    : handle_{handle, &dlclose}
  {
  }

  std::unique_ptr<void, decltype(&dlclose)> handle_;

  friend class DLLoader;
};
} // namespace redoom::Utils
