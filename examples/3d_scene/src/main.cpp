// #include <iostream>
//
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <stb/stb_image.h>
//
// #include <Utils/Expected.hh>
// #include <redoom/graphics/Buffer.hh>
// #include <redoom/graphics/Camera.hh>
// #include <redoom/graphics/Mesh.hh>
// #include <redoom/graphics/Program.hh>
// #include <redoom/graphics/Shader.hh>
// #include <redoom/graphics/Texture.hh>
// #include <redoom/graphics/Vertex.hh>
// #include <redoom/graphics/VertexArray.hh>
// #include <redoom/graphics/mesh/Torus.hh>
//
// using redoom::Expected;
// using redoom::make_formatted_unexpected;
// using redoom::graphics::Camera;
// using redoom::graphics::FragmentShader;
// using redoom::graphics::Program;
// using redoom::graphics::Texture2D;
// using redoom::graphics::VertexShader;
// using redoom::graphics::mesh::Torus;
//
// static auto camera = Camera{glm::vec3(0.0f, 0.0f, 3.0f)}; // NOLINT
//
// static void framebuffer_size_callback(
//     GLFWwindow* /*window*/, int width, int height)
// {
//   glViewport(0, 0, width, height);
// }
//
// static void scroll_callback(
//     GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
// {
//   auto fov = camera.getFov();
//   fov -= static_cast<float>(yoffset);
//   if (fov < 1.0f)
//     fov = 1.0f;
//   if (fov > 45.0f)
//     fov = 45.0f;
//   camera.setFov(fov);
// }
//
// static void mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos)
// {
//   static auto first_mouse = true;
//   static auto last_x = 0.0f;
//   static auto last_y = 0.0f;
//
//   if (first_mouse) {
//     last_x = static_cast<float>(xpos);
//     last_y = static_cast<float>(ypos);
//     first_mouse = false;
//   }
//
//   auto xoffset = static_cast<float>(xpos) - last_x;
//   auto yoffset = last_y - static_cast<float>(ypos);
//   last_x = static_cast<float>(xpos);
//   last_y = static_cast<float>(ypos);
//
//   auto const sensitivity = camera.getSensitivity();
//   xoffset *= sensitivity;
//   yoffset *= sensitivity;
//
//   auto yaw = camera.getYaw();
//   auto pitch = camera.getPitch();
//   yaw += xoffset;
//   pitch += yoffset;
//   if (pitch > 89.0f)
//     pitch = 89.0f;
//   if (pitch < -89.0f)
//     pitch = -89.0f;
//   camera.setYaw(yaw);
//   camera.setPitch(pitch);
//
//   auto direction =
//       glm::vec3{std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
//           std::sin(glm::radians(pitch)),
//           std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))};
//   // camera_front = glm::normalize(direction);
// }
//
// static void processInput(GLFWwindow* window)
// {
//   static auto is_pressed = false;
//   static auto is_wireframe = false;
//
//   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//     glfwSetWindowShouldClose(window, 1);
//   else if (!is_pressed && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//     is_pressed = true;
//     is_wireframe = !is_wireframe;
//     glPolygonMode(GL_FRONT_AND_BACK, is_wireframe ? GL_LINE : GL_FILL);
//   } else if (is_pressed && glfwGetKey(window, GLFW_KEY_SPACE) ==
//   GLFW_RELEASE)
//     is_pressed = false;
//
//   auto camera_pos = camera.getPosition();
//   auto const camera_speed = camera.getSpeed();
//   auto const camera_front = camera.getFront();
//   auto const camera_up = camera.getUp();
//   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//     camera_pos += camera_speed * camera_front;
//   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     camera_pos -= camera_speed * camera_front;
//   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//     camera_pos -=
//         glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
//   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     camera_pos +=
//         glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
//   camera.setPosition(camera_pos);
// }
//
// static Expected<int> expMain() noexcept
// {
//   glfwInit();
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//   glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//   GLFWwindow* window = glfwCreateWindow(800, 600, "3D Scene", nullptr,
//   nullptr); if (window == nullptr)
//     return tl::make_unexpected("Failed to create GLFW window");
//   glfwMakeContextCurrent(window);
//   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//   // glfwSwapInterval(0); // Disable vsync
//   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//   glfwSetScrollCallback(window, scroll_callback);
//   glfwSetCursorPosCallback(window, mouse_callback);
//
//   glewExperimental = GL_TRUE;
//   GLenum err = glewInit();
//   if (err != GLEW_OK)
//     return make_formatted_unexpected("{}", glewGetErrorString(err));
//   glEnable(GL_DEPTH_TEST);
//
//   // Link the shaders to a program
//   auto vertex_shader_exp =
//       VertexShader::fromFile("../examples/3d_scene/shaders/vs.glslx");
//   RETURN_IF_UNEXPECTED(vertex_shader_exp);
//   auto& vertex_shader = *vertex_shader_exp;
//   auto fragment_shader_exp =
//       FragmentShader::fromFile("../examples/3d_scene/shaders/fs.glslx");
//   RETURN_IF_UNEXPECTED(vertex_shader_exp);
//   auto& fragment_shader = *fragment_shader_exp;
//   auto program_exp = Program::create(vertex_shader, fragment_shader);
//   RETURN_IF_UNEXPECTED(program_exp);
//   auto& program = *program_exp;
//
//   // clang-format off
//   auto objects_data = std::array{
//     glm::vec3( 0.0f,  0.0f,  0.0f),
//     glm::vec3( 2.0f,  5.0f, -15.0f),
//     glm::vec3(-1.5f, -2.2f, -2.5f),
//     glm::vec3(-3.8f, -2.0f, -12.3f),
//     glm::vec3( 2.4f, -0.4f, -3.5f),
//     glm::vec3(-1.7f,  3.0f, -7.5f),
//     glm::vec3( 1.3f, -2.0f, -2.5f),
//     glm::vec3( 1.5f,  2.0f, -2.5f),
//     glm::vec3( 1.5f,  0.2f, -1.5f),
//     glm::vec3(-1.3f,  1.0f, -1.5f)
//   };
//   // clang-format on
//
//   // Create texture from file
//   auto donut_tex_exp = Texture2D::fromFile("../assets/donut.jpg");
//   RETURN_IF_UNEXPECTED(donut_tex_exp);
//   auto& donut_tex = *donut_tex_exp;
//   auto textures = std::vector<Texture2D>{};
//   textures.push_back(std::move(donut_tex));
//
//   auto mesh =
//       Torus{1.0f, 0.5f, 100, 100, {1.0f, 1.0f, 1.0f}, std::move(textures)};
//
//   auto previous_time = glfwGetTime();
//   auto frames = 0;
//   while (glfwWindowShouldClose(window) == 0) {
//     auto current_time = glfwGetTime();
//     ++frames;
//     if (current_time - previous_time > 1.0) {
//       std::cout << frames << '\n';
//       previous_time = current_time;
//       frames = 0;
//     }
//
//     processInput(window);
//
//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//     auto projection = glm::perspective(
//         glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);
//     program.setUniformMatrix4(
//         "projection", 1, GL_FALSE, glm::value_ptr(projection));
//
//     auto const view = camera.getView();
//     program.setUniformMatrix4("view", 1, GL_FALSE, glm::value_ptr(view));
//
//     for (auto i = 0u; i < objects_data.size(); ++i) {
//       auto const& pos = objects_data[i];
//       auto model = glm::mat4(1.0f);
//       model = glm::translate(model, pos);
//       auto angle = 20.0f * static_cast<float>(i);
//       model =
//           glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f,
//           0.5f));
//       program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
//       mesh.draw(program);
//     }
//
//     glfwSwapBuffers(window);
//     glfwPollEvents();
//   }
//   return 0;
// }
//
// int main()
// {
//   auto exp = expMain();
//   if (!exp) {
//     std::cerr << exp.error() << '\n';
//     return 1;
//   } else
//     return *exp;
// }

#include <redoom/Application.hh>
#include <redoom/EntryPoint.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>
#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/ecs/systems/BehaviourSystem.hh>
#include <redoom/ecs/systems/CameraSystem.hh>
#include <redoom/ecs/systems/MeshSystem.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/mesh/Torus.hh>

#include <behaviours/DonutBehaviour.hh>
#include <behaviours/FPSCounterBehaviour.hh>

using redoom::ecs::SystemPriority;
using redoom::ecs::components::CameraComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::TransformComponent;
using redoom::ecs::systems::BehaviourSystem;
using redoom::ecs::systems::CameraSystem;
using redoom::ecs::systems::MeshSystem;
using redoom::graphics::ShaderLibrary;
using redoom::graphics::Texture2D;
using redoom::graphics::mesh::Torus;

namespace redoom
{
struct ExampleApplication : public Application {
  explicit ExampleApplication(ApplicationArguments pargs) noexcept
    : Application("3D Scene example", pargs)
  {
  }
};

std::unique_ptr<Application> createApplication(ApplicationArguments args)
{
  auto app = std::make_unique<ExampleApplication>(args);

  auto exp = ShaderLibrary::addShader("default",
      "../examples/3d_scene/shaders/vs.glslx",
      "../examples/3d_scene/shaders/fs.glslx");
  if (!exp) {
    std::cerr << exp.error() << '\n';
    assert(false);
  }

  auto tex_exp = Texture2D::fromFile("../assets/donut.jpg");
  if (!tex_exp) {
    std::cerr << exp.error() << '\n';
    assert(false);
  }
  auto& tex = *tex_exp;
  auto textures = std::vector<Texture2D>{};
  textures.push_back(std::move(tex));

  auto mesh = std::shared_ptr<Torus>(
      new Torus{1.0f, 0.5f, 100, 100, {1.0f, 1.0f, 1.0f}, std::move(textures)});

  auto& registry = app->getRegistry();
  auto fps_counter = registry.makeEntity();
  registry.attachComponent<FPSCounterBehaviour>(fps_counter);
  auto camera = registry.makeEntity();
  registry.attachComponent<CameraComponent>(
      camera, graphics::Camera(glm::vec3{0.0f, 0.0f, 3.0f}));
  auto donut = registry.makeEntity();
  registry.attachComponent<DonutBehaviour>(donut);
  registry.attachComponent<MeshComponent>(donut, mesh);
  registry.attachComponent<TransformComponent>(donut,
      TransformComponent(
          {0.0, 0.0, -5.0}, glm::radians(25.0f), {1.0f, 1.0f, 0.0f}));
  registry.attachSystem<BehaviourSystem>(SystemPriority{0});
  registry.attachSystem<CameraSystem>(SystemPriority{0});
  registry.attachSystem<MeshSystem>(SystemPriority{1000});

  return app;
}
} // namespace redoom
