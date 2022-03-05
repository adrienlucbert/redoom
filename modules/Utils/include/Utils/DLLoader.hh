#pragma once

#include <dlfcn.h>
#include <memory>
#include <string_view>

#include <Utils/Expected.hh>

namespace redoom::Utils
{
class DLLoader
{
public:
  template <typename SymT>
  Expected<SymT> load(std::string_view symbol) const
  {
    auto* sym = dlsym(this->handle_.get(), symbol.data());
    if (sym == nullptr)
      return make_formatted_unexpected("{}: symbol not found", symbol);
    return (reinterpret_cast<SymT>(sym)); // NOLINT
  }

  static Expected<DLLoader> open(std::string_view filepath) noexcept
  {
    auto* handle = dlopen(filepath.data(), RTLD_LAZY);
    if (handle == nullptr) {
      return make_formatted_unexpected("{}: could not open library", filepath);
    }
    return {DLLoader{handle}};
  }

private:
  explicit DLLoader(void* handle) noexcept
    : handle_{handle, &dlclose}
  {
  }

  std::unique_ptr<void, decltype(&dlclose)> handle_;
};
} // namespace redoom::Utils
