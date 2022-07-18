#include <redoom/graphics/Mesh.hh>

#include <iostream>

#include <tl/expected.hpp>

namespace redoom::graphics
{
Mesh::Mesh(
    std::vector<Vertex> vertices, std::vector<unsigned int> indices) noexcept
  : vertices_{std::move(vertices)}
  , indices_{std::move(indices)}
  , vbo_{this->vertices_, BufferUsage::STATIC}
  , ebo_{this->indices_, BufferUsage::STATIC}
{
  this->vao_.bind();
  this->vbo_.bind();
  this->ebo_.bind();
  this->vbo_.setLayout({{ShaderDataType::Float3, "aPos"},
      {ShaderDataType::Float3, "aNormal"},
      {ShaderDataType::Float2, "aTexCoord"}});
  this->vao_.unbind();
  this->vbo_.unbind();
  this->ebo_.unbind();
}

void Mesh::draw() const noexcept
{
  this->vao_.bind();
  if (!this->indices_.empty())
    this->ebo_.draw();
  else
    this->vbo_.draw();
  this->vao_.unbind();
}

std::string const& Mesh::getType() const noexcept
{
  static auto const type = std::string{"Custom"};
  return type;
}

std::vector<Vertex> const& Mesh::getVertices() const noexcept
{
  return this->vertices_;
}

std::vector<unsigned int> const& Mesh::getIndices() const noexcept
{
  return this->indices_;
}
} // namespace redoom::graphics
