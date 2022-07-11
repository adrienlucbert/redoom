#pragma once

#include <filesystem>
#include <unordered_map>

#include <Utils/DynLibrary.hh>
#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>

namespace redoom::Utils
{
class DLLoader : public Singleton<DLLoader>
{
public:
  Expected<std::reference_wrapper<DynLibrary const>> getLibrary(
      std::filesystem::path const& filepath) noexcept
  {
    auto dll_it = this->dlls_.find(filepath);
    if (dll_it != this->dlls_.end())
      return dll_it->second;

    auto* handle = dlopen(filepath.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
      return make_formatted_unexpected(
          "{}: could not open library", filepath.c_str());
    }
    auto [it, success] = this->dlls_.emplace(filepath, DynLibrary{handle});
    if (!success) {
      return make_formatted_unexpected(
          "{}: could not add library to DLLoader", filepath.c_str());
    }
    return it->second;
  }

protected:
  DLLoader() noexcept = default;

  std::unordered_map<std::string, DynLibrary> dlls_{}; // NOLINT
};
} // namespace redoom::Utils
