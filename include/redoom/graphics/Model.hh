#pragma once

#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <tl/optional.hpp>

#include <Utils/Expected.hh>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>

namespace redoom::graphics
{
struct ModelImporterOptions {
  bool flip_uvs{false};
};

class Model
{
public:
  struct SubModel {
    Mesh mesh_;
    std::vector<std::reference_wrapper<Texture2D>> textures_;
  };

  Model(Model const&) noexcept = default;
  Model(Model&&) noexcept = default;
  ~Model() noexcept = default;

  Model& operator=(Model const&) noexcept = default;
  Model& operator=(Model&&) noexcept = default;

  static Expected<Model> fromFile(std::filesystem::path path,
      ModelImporterOptions const& options = {}) noexcept;

  void draw() const noexcept;

  [[nodiscard]] tl::optional<std::filesystem::path> const& getPath()
      const noexcept;
  [[nodiscard]] tl::optional<ModelImporterOptions> const& getImporterOptions()
      const noexcept;
  [[nodiscard]] std::vector<SubModel> const& getSubModels() const noexcept;

private:
  explicit Model(std::filesystem::path path,
      ModelImporterOptions importer_options,
      std::vector<SubModel> submodels) noexcept;

  static Expected<std::vector<SubModel>> parseNode(aiNode* node,
      aiScene const* scene,
      std::filesystem::path const& root) noexcept;
  static Expected<SubModel> parseMesh(aiMesh* mesh,
      aiScene const* scene,
      std::filesystem::path const& root) noexcept;
  static Expected<std::vector<std::reference_wrapper<Texture2D>>>
  loadMaterialTexture(aiMaterial* mat,
      aiTextureType ai_type,
      Texture2D::Type tex_type,
      std::filesystem::path const& root) noexcept;

  tl::optional<std::filesystem::path> path_;
  tl::optional<ModelImporterOptions> importer_options_;
  std::vector<SubModel> submodels_;
};
} // namespace redoom::graphics
