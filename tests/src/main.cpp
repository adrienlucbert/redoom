#include <catch2/catch_session.hpp>

int main(int argc, char const* const* argv)
{
  return Catch::Session().run(argc, argv);
}
