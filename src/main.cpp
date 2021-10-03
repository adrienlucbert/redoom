#include <redoom/Application.hh>

#include <redoom/ecs/components/MeshComponent.hh>

using redoom::Application;
using redoom::ApplicationArguments;

namespace redoom
{
struct EditorApplication : public Application {
  explicit EditorApplication(ApplicationArguments pargs) noexcept
    : Application("Redoom editor", pargs)
  {
  }
};

std::unique_ptr<Application> createApplication(ApplicationArguments args)
{
  auto app = std::make_unique<EditorApplication>(args);
  return app;
}
} // namespace redoom

#include <redoom/EntryPoint.hh>
