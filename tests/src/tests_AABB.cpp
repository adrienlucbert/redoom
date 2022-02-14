#include <catch2/catch.hpp>

#include <redoom/physics/AABB.hh>

using redoom::physics::AABB;

TEST_CASE("[AABB] Basic tests", "[Physics][AABB]")
{
  auto a = AABB{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}; // base
  auto b = AABB{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}}; // a + 0.5
  auto c = AABB{{1.5f, 1.5f, 1.5f}, {2.5f, 2.5f, 2.5f}}; // a + 1.5
  auto d = AABB{{2.0f, 2.0f, 2.0f}, {6.0f, 6.0f, 6.0f}}; // b * 4.0

  SECTION("Operators")
  {
    SECTION("operator== and operator!=")
    {
      CHECK(a == AABB{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}});
      CHECK(a != b);
    }
    SECTION("operator+ and operator+=")
    {
      CHECK(a + 0.5f == b);
      CHECK(a + glm::vec3{0.5f, 0.5f, 0.5f} == b);

      a += 0.5f;
      CHECK(a == AABB{{0.5f, 0.5f, 0.5f}, {1.5f, 1.5f, 1.5f}});
    }
    SECTION("operator- and operator-=")
    {
      CHECK(b - 0.5f == a);
      CHECK(b - glm::vec3{0.5f, 0.5f, 0.5f} == a);

      a -= 0.5f;
      CHECK(a == AABB{{-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}});
    }
    SECTION("operator* and operator*=")
    {
      CHECK(b * 4.0f == d);

      a *= 2.0f;
      CHECK(a == AABB{{0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}});
    }
    SECTION("operator/ and operator/=")
    {
      CHECK(d / 4.0f == b);

      b /= 2.0f;
      CHECK(b == AABB{{0.25f, 0.25f, 0.25f}, {0.75f, 0.75f, 0.75f}});
    }
  }

  SECTION("Methods")
  {
    SECTION("getCenter")
    {
      CHECK(a.getCenter() == glm::vec3{0.5f, 0.5f, 0.5f});
    }

    SECTION("combine")
    {
      auto e = b.combine(c);
      CHECK(b == b); // b shouldn't change
      CHECK(e == AABB{{0.5f, 0.5f, 0.5f}, {2.5f, 2.5f, 2.5f}});
      e = e.combine(d);
      CHECK(e == AABB{{0.5f, 0.5f, 0.5f}, {6.0f, 6.0f, 6.0f}});
      e = e.combine(a);
      CHECK(e == AABB{{0.0f, 0.0f, 0.0f}, {6.0f, 6.0f, 6.0f}});
    }

    SECTION("contains")
    {
      auto container = AABB{a.lower_bounds, d.upper_bounds};
      auto half_inside = AABB{{3.0f, 3.0f, 3.0f}, {9.0f, 9.0f, 9.0f}};
      CHECK(container.contains(a) == true);            // touching lower border
      CHECK(container.contains(b) == true);            // full inside
      CHECK(container.contains(half_inside) == false); // half inside

      CHECK(container.contains(glm::vec3{0.5f, 0.5f, 0.5f}) == true);
      CHECK(container.contains(glm::vec3{10.0f, 10.0f, 10.0f}) == false);
    }

    SECTION("intersects")
    {
      CHECK(a.intersects(b) == true);
      CHECK(b.intersects(c) == true);
      CHECK(a.intersects(c) == false);
    }
  }
}
