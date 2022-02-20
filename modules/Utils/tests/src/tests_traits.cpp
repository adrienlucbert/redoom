#include <catch2/catch.hpp>

#include <Utils/traits.hpp>
#include <mock/traits.hpp>

TEST_CASE("[is_constructible] Basic tests", "[Utils][traits][is_constructible]")
{
  SECTION("With public constructors")
  {
    SECTION("Default constructible")
    {
      CHECK(
          redoom::Utils::is_default_constructible_v<mock::
                  PublicDefaultConstructible> == std::is_default_constructible_v<mock::PublicDefaultConstructible>);
    }

    SECTION("Default constructible with extra arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::PublicDefaultConstructible,
              int> == std::is_constructible_v<mock::PublicDefaultConstructible, int>);
    }

    SECTION("Non default constructible without arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::
                  PublicNonDefaultConstructible> == std::is_constructible_v<mock::PublicNonDefaultConstructible>);
    }

    SECTION("Non default constructible with valid arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::PublicNonDefaultConstructible,
              int,
              float> == std::is_constructible_v<mock::PublicNonDefaultConstructible, int, float>);
    }

    SECTION("Non default constructible with implicitly (convertible) arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::PublicNonDefaultConstructible,
              int,
              double> == std::is_constructible_v<mock::PublicNonDefaultConstructible, int, double>);
    }

    SECTION("Non default constructible with invalid arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::PublicNonDefaultConstructible,
              std::string,
              long> == std::is_constructible_v<mock::PublicNonDefaultConstructible, std::string, long>);
    }
  }

  SECTION("With protected constructors")
  {
    SECTION("Default constructible")
    {
      CHECK(
          redoom::Utils::is_default_constructible_v<mock::
                  ProtectedDefaultConstructible> == std::is_default_constructible_v<mock::PublicDefaultConstructible>);
    }

    SECTION("Default constructible with extra arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::ProtectedDefaultConstructible,
              int> == std::is_constructible_v<mock::PublicDefaultConstructible, int>);
    }

    SECTION("Non default constructible without arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::
                  ProtectedNonDefaultConstructible> == std::is_constructible_v<mock::PublicNonDefaultConstructible>);
    }

    SECTION("Non default constructible with valid arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<
              mock::ProtectedNonDefaultConstructible,
              int,
              float> == std::is_constructible_v<mock::PublicNonDefaultConstructible, int, float>);
    }

    SECTION("Non default constructible with implicitly (convertible) arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<
              mock::ProtectedNonDefaultConstructible,
              int,
              double> == std::is_constructible_v<mock::PublicNonDefaultConstructible, int, double>);
    }

    SECTION("Non default constructible with invalid arguments")
    {
      CHECK(
          redoom::Utils::is_constructible_v<mock::ProtectedDefaultConstructible,
              std::string,
              long> == std::is_constructible_v<mock::PublicNonDefaultConstructible, std::string, long>);
    }
  }
}
