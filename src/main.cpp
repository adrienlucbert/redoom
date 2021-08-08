#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <Utils/Expected.hh>
#include <redoom/graphics/Buffer.hh>
#include <redoom/graphics/Camera.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/Shader.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/VertexArray.hh>

using redoom::Expected;
using redoom::make_formatted_unexpected;
using redoom::graphics::BufferUsage;
using redoom::graphics::Camera;
using redoom::graphics::FragmentShader;
using redoom::graphics::Program;
using redoom::graphics::Texture2D;
using redoom::graphics::VertexArray;
using redoom::graphics::VertexBuffer;
using redoom::graphics::VertexShader;

auto camera = Camera{glm::vec3(0.0f, 0.0f, 3.0f)}; // NOLINT

static void framebuffer_size_callback(
    GLFWwindow* /*window*/, int width, int height)
{
  glViewport(0, 0, width, height);
}

static void scroll_callback(
    GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
  auto fov = camera.getFov();
  fov -= static_cast<float>(yoffset);
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
  camera.setFov(fov);
}

static void mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
  static auto first_mouse = true;
  static auto last_x = 0.0f;
  static auto last_y = 0.0f;

  if (first_mouse) {
    last_x = static_cast<float>(xpos);
    last_y = static_cast<float>(ypos);
    first_mouse = false;
  }

  auto xoffset = static_cast<float>(xpos) - last_x;
  auto yoffset = last_y - static_cast<float>(ypos);
  last_x = static_cast<float>(xpos);
  last_y = static_cast<float>(ypos);

  auto const sensitivity = camera.getSensitivity();
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  auto yaw = camera.getYaw();
  auto pitch = camera.getPitch();
  yaw += xoffset;
  pitch += yoffset;
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  camera.setYaw(yaw);
  camera.setPitch(pitch);

  auto direction =
      glm::vec3{std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
          std::sin(glm::radians(pitch)),
          std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))};
  // camera_front = glm::normalize(direction);
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

  auto camera_pos = camera.getPosition();
  auto const camera_speed = camera.getSpeed();
  auto const camera_front = camera.getFront();
  auto const camera_up = camera.getUp();
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_pos += camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_pos -= camera_speed * camera_front;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_pos -=
        glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_pos +=
        glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
  camera.setPosition(camera_pos);
}

static Expected<int> expMain() noexcept
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return tl::make_unexpected("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSwapInterval(0); // Disable vsync
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    return make_formatted_unexpected("{}", glewGetErrorString(err));
  glEnable(GL_DEPTH_TEST);

  // Link the shaders to a program
  auto vertex_shader_exp = VertexShader::fromFile("../glsl/vs.glslx");
  RETURN_IF_UNEXPECTED(vertex_shader_exp);
  auto& vertex_shader = *vertex_shader_exp;
  auto fragment_shader_exp = FragmentShader::fromFile("../glsl/fs.glslx");
  RETURN_IF_UNEXPECTED(vertex_shader_exp);
  auto& fragment_shader = *fragment_shader_exp;
  auto program_exp = Program::create(vertex_shader, fragment_shader);
  RETURN_IF_UNEXPECTED(program_exp);
  auto& program = *program_exp;

  // clang-format off
  auto vertices = std::array{
    // positions          // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  auto cube_positions = std::array<glm::vec3, 10>{
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  // clang-format on

  // Create texture from file
  stbi_set_flip_vertically_on_load(1);
  auto tex0_exp = Texture2D::fromFile("../assets/box.png");
  RETURN_IF_UNEXPECTED(tex0_exp);
  auto& tex0 = *tex0_exp;

  // Create the vertex array object
  auto vao = VertexArray{};
  vao.bind();

  // Create the vertex buffer object
  auto vbo = VertexBuffer{vertices, vertices.size() / 5, BufferUsage::STATIC};
  vbo.bind();
  // position attribute
  glVertexAttribPointer(0,
      3,
      GL_FLOAT,
      GL_FALSE,
      5 * sizeof(float),
      (void*)(0 * sizeof(float))); // NOLINT
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1,
      2,
      GL_FLOAT,
      GL_FALSE,
      5 * sizeof(float),
      (void*)(3 * sizeof(float))); // NOLINT
  glEnableVertexAttribArray(1);

  // Unbind the vertex array object and vertex buffer object
  vbo.unbind();
  VertexArray::unbind();

  program.use();
  program.setUniform("texture0", 0);

  auto previous_time = glfwGetTime();
  while (glfwWindowShouldClose(window) == 0) {
    auto current_time = glfwGetTime();
    // std::cout << static_cast<int>(1.0 / (current_time - previous_time)) <<
    // '\n';
    previous_time = current_time;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tex0.setUnit(GL_TEXTURE0);
    tex0.bind();

    auto projection = glm::perspective(
        glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
    program.setUniformMatrix4(
        "projection", 1, GL_FALSE, glm::value_ptr(projection));

    auto const view = camera.getView();
    program.setUniformMatrix4("view", 1, GL_FALSE, glm::value_ptr(view));
    vao.bind();
    vbo.bind();
    for (auto i = 0u; i < 10; ++i) {
      auto const& pos = cube_positions[i];
      auto model = glm::mat4(1.0f);
      model = glm::translate(model, pos);
      auto angle = 20.0f * static_cast<float>(i);
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
      vbo.draw();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

int main()
{
  auto exp = expMain();
  if (!exp) {
    std::cerr << exp.error() << '\n';
    return 1;
  } else
    return *exp;
}
