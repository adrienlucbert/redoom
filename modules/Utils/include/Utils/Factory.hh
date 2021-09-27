#pragma once

#include <memory>

#include <Utils/DLLoader.hh>
#include <Utils/Expected.hh>

namespace redoom::Utils
{
template <typename T, typename Allocator = std::unique_ptr<T> (*)()>
class Factory
{
public:
  Expected<Factory> static fromDL(
      std::string_view filepath, std::string_view symbol) noexcept
  {
    auto loader_exp = DLLoader::open(filepath);
    RETURN_IF_UNEXPECTED(loader_exp);
    auto& loader = *loader_exp;
    auto factory_exp = loader.load<Allocator>(symbol);
    RETURN_IF_UNEXPECTED(factory_exp);
    return {Factory{std::move(loader), *factory_exp}};
  }

  template <typename... Args>
  auto make(Args&&... args) const noexcept
  {
    return std::invoke(this->allocator, std::forward<Args>(args)...);
  }

private:
  explicit Factory(DLLoader ploader, Allocator pallocator) noexcept
    : loader{std::move(ploader)}
    , allocator{pallocator}
  {
  }

  DLLoader loader;
  Allocator allocator;
};
} // namespace redoom::Utils
