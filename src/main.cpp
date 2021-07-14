#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/graphics/Buffer.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/Shader.hh>
#include <redoom/graphics/VertexArray.hh>

using redoom::graphics::BufferUsage;
using redoom::graphics::FragmentShader;
using redoom::graphics::IndexBuffer;
using redoom::graphics::Program;
using redoom::graphics::VertexArray;
using redoom::graphics::VertexBuffer;
using redoom::graphics::VertexShader;

static void framebuffer_size_callback(
    GLFWwindow* /*window*/, int width, int height)
{
  glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
  static auto is_pressed = false;
  static auto is_wireframe = false;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
  else if (!is_pressed && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    is_pressed = true;
    is_wireframe = !is_wireframe;
    glPolygonMode(GL_FRONT_AND_BACK, is_wireframe ? GL_LINE : GL_FILL);
  } else if (is_pressed && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    is_pressed = false;
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

  // Link the shaders to a program
  auto program = Program::create(VertexShader::fromFile("../glsl/vs.glslx"),
      FragmentShader::fromFile("../glsl/fs.glslx"));

  // clang-format off
  auto vertices = std::array{
       0.5f,  0.5f, 0.0f,  // top right
       0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left
  };
  auto indices = std::array{
      0u, 1u, 3u,   // first triangle
      1u, 2u, 3u    // second triangle
  };
  // clang-format on

  // Create the element buffer object
  auto ebo = IndexBuffer{indices, BufferUsage::STATIC};

  // Create the vertex array object
  auto vao = VertexArray{};
  vao.bind();

  // Create the vertex buffer object
  auto vbo = VertexBuffer{vertices, BufferUsage::STATIC};
  vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // Unbind the vertex array object and vertex buffer object
  vbo.unbind();
  VertexArray::unbind();

  while (glfwWindowShouldClose(window) == 0) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.use();
    auto value = (static_cast<float>(std::sin(glfwGetTime())) / 2.0f) + 0.5f;
    program.setUniform("fillColor", 0.0f, 1.0f, value);

    vao.bind();
    ebo.bind();
    ebo.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
