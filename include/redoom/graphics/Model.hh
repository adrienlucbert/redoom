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
  Model(Model const& b) noexcept = default;
  Model(Model&& b) noexcept = default;
  ~Model() noexcept = default;

  Model& operator=(Model const& rhs) noexcept = default;
  Model& operator=(Model&& rhs) noexcept = default;

  static Expected<Model> fromFile(std::filesystem::path path,
      ModelImporterOptions const& options = {}) noexcept;

  void draw(Program& program) const noexcept;

  [[nodiscard]] tl::optional<std::filesystem::path> const& getPath()
      const noexcept;
  [[nodiscard]] tl::optional<ModelImporterOptions> const& getImporterOptions()
      const noexcept;
  [[nodiscard]] std::vector<Mesh> const& getMeshes() const noexcept;

private:
  explicit Model(std::filesystem::path path,
      ModelImporterOptions importer_options,
      std::vector<Mesh> meshes) noexcept;

  static Expected<std::vector<Mesh>> parseNode(aiNode* node,
      aiScene const* scene,
      std::filesystem::path const& root) noexcept;
  static Expected<Mesh> parseMesh(aiMesh* mesh,
      aiScene const* scene,
      std::filesystem::path const& root) noexcept;
  static Expected<std::vector<Texture2D>> loadMaterialTexture(aiMaterial* mat,
      aiTextureType ai_type,
      Texture2D::Type tex_type,
      std::filesystem::path const& root) noexcept;

  tl::optional<std::filesystem::path> path_;
  tl::optional<ModelImporterOptions> importer_options_;
  std::vector<Mesh> meshes_;
};
} // namespace redoom::graphics
