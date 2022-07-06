#include <redoom/graphics/Model.hh>

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <tl/expected.hpp>

#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics
{
Expected<Model> Model::fromFile(std::filesystem::path path,
    ModelImporterOptions const& importer_options) noexcept
{
  auto importer = Assimp::Importer{};
  auto importer_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals;
  if (importer_options.flip_uvs)
    importer_flags |= aiProcess_FlipUVs;

  auto const* scene = importer.ReadFile(path, importer_flags);
  if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u)
      || (scene->mRootNode == nullptr))
    return make_formatted_unexpected(
        "Couldn't import model: {}", importer.GetErrorString());

  auto model_path = path;
  auto& root = path.remove_filename();
  auto exp = Model::parseNode(scene->mRootNode, scene, root);
  RETURN_IF_UNEXPECTED(exp);
  return Model{std::move(model_path), importer_options, std::move(*exp)};
}

// NOLINTNEXTLINE(misc-no-recursion)
Expected<std::vector<Mesh>> Model::parseNode(aiNode* node,
    aiScene const* scene,
    std::filesystem::path const& root) noexcept
{
  auto meshes = std::vector<Mesh>{};

  for (auto i = 0u; i < node->mNumMeshes; ++i) {
    auto* mesh = scene->mMeshes[node->mMeshes[i]];
    auto exp = Model::parseMesh(mesh, scene, root);
    RETURN_IF_UNEXPECTED(exp);
    meshes.push_back(std::move(*exp));
  }
  for (auto i = 0u; i < node->mNumChildren; ++i) {
    auto exp = Model::parseNode(node->mChildren[i], scene, root);
    RETURN_IF_UNEXPECTED(exp);
    std::move((*exp).begin(), (*exp).end(), std::back_inserter(meshes));
  }
  return meshes;
}

Expected<Mesh> Model::parseMesh(aiMesh* mesh,
    aiScene const* scene,
    std::filesystem::path const& root) noexcept
{
  auto vertices = std::vector<Vertex>{};
  auto indices = std::vector<unsigned int>{};
  auto textures = std::vector<Texture2D>{};

  // parse vertices
  for (auto i = 0u; i < mesh->mNumVertices; ++i) {
    auto vertex = Vertex{};

    vertex.position.x = mesh->mVertices[i].x; // NOLINT
    vertex.position.y = mesh->mVertices[i].y; // NOLINT
    vertex.position.z = mesh->mVertices[i].z; // NOLINT

    vertex.normal.x = mesh->mNormals[i].x; // NOLINT
    vertex.normal.y = mesh->mNormals[i].y; // NOLINT
    vertex.normal.z = mesh->mNormals[i].z; // NOLINT

    vertex.color.x = 1.0f; // NOLINT
    vertex.color.y = 1.0f; // NOLINT
    vertex.color.z = 1.0f; // NOLINT

    if (mesh->mTextureCoords[0] != nullptr) {
      vertex.texUV.x = mesh->mTextureCoords[0][i].x; // NOLINT
      vertex.texUV.y = mesh->mTextureCoords[0][i].y; // NOLINT
    }

    vertices.push_back(vertex);
  }
  // parse indices
  for (auto i = 0u; i < mesh->mNumFaces; ++i) {
    for (auto j = 0u; j < mesh->mFaces[i].mNumIndices; ++j) {
      indices.push_back(mesh->mFaces[i].mIndices[j]);
    }
  }
  // parse materials
  auto* material = scene->mMaterials[mesh->mMaterialIndex];

  static auto const texture_types =
      std::map<enum aiTextureType, Texture2D::Type>{
          {aiTextureType_DIFFUSE, Texture2D::Type::Diffuse},
          {aiTextureType_SPECULAR, Texture2D::Type::Specular},
          {aiTextureType_AMBIENT, Texture2D::Type::Ambient},
          {aiTextureType_EMISSIVE, Texture2D::Type::Emissive},
          {aiTextureType_HEIGHT, Texture2D::Type::Height},
          {aiTextureType_NORMALS, Texture2D::Type::Normals},
          {aiTextureType_SHININESS, Texture2D::Type::Shininess},
          {aiTextureType_OPACITY, Texture2D::Type::Opacity},
          {aiTextureType_DISPLACEMENT, Texture2D::Type::Displacement},
          {aiTextureType_LIGHTMAP, Texture2D::Type::Lightmap},
          {aiTextureType_REFLECTION, Texture2D::Type::Reflection},
          {aiTextureType_BASE_COLOR, Texture2D::Type::BaseColor},
      };

  for (auto const& [assimp_type, texture_type] : texture_types) {
    auto exp =
        Model::loadMaterialTexture(material, assimp_type, texture_type, root);
    RETURN_IF_UNEXPECTED(exp);
    std::move((*exp).begin(), (*exp).end(), std::back_inserter(textures));
  }

  return Mesh{std::move(vertices), std::move(indices), std::move(textures)};
}

Expected<std::vector<Texture2D>> Model::loadMaterialTexture(aiMaterial* mat,
    aiTextureType ai_type,
    Texture2D::Type tex_type,
    std::filesystem::path const& root) noexcept
{
  auto textures = std::vector<Texture2D>{};

  for (auto i = 0u; i < mat->GetTextureCount(ai_type); ++i) {
    auto str = aiString{};
    mat->GetTexture(ai_type, i, &str);
    auto exp = Texture2D::fromFile(root / str.C_Str(), tex_type);
    RETURN_IF_UNEXPECTED(exp);
    textures.push_back(std::move(*exp));
  }
  return textures;
}

Model::Model(std::filesystem::path path,
    ModelImporterOptions importer_options,
    std::vector<Mesh> meshes) noexcept
  : path_{std::move(path)}
  , importer_options_{importer_options}
  , meshes_{std::move(meshes)}
{
}

void Model::draw(Program& program) const noexcept
{
  for (auto const& mesh : this->meshes_)
    mesh.draw(program);
}

tl::optional<std::filesystem::path> const& Model::getPath() const noexcept
{
  return this->path_;
}

tl::optional<ModelImporterOptions> const& Model::getImporterOptions()
    const noexcept
{
  return this->importer_options_;
}

std::vector<Mesh> const& Model::getMeshes() const noexcept
{
  return this->meshes_;
}
} // namespace redoom::graphics
