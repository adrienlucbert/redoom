#include <redoom/Application.hh>

int main(int argc, char** argv)
{
  auto app = redoom::createApplication({argc, argv});
  if (!app->isReady())
    return 1;
  app->run();
  return 0;
}
