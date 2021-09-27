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
    auto* sym = dlsym(this->handle.get(), symbol.data());
    if (sym == nullptr)
      return make_formatted_unexpected("Symbol {} not found", symbol);
    return (reinterpret_cast<SymT>(sym)); // NOLINT
  }

  static Expected<DLLoader> open(std::string_view filepath) noexcept
  {
    auto* handle = dlopen(filepath.data(), RTLD_LAZY);
    if (handle == nullptr) {
      return tl::make_unexpected("could not open library");
    }
    return {DLLoader{handle}};
  }

private:
  explicit DLLoader(void* phandle) noexcept
    : handle{phandle, &dlclose}
  {
  }

  std::unique_ptr<void, decltype(&dlclose)> handle;
};
} // namespace redoom::Utils
