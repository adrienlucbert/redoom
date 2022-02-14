#include <catch2/catch.hpp>

#include <iostream>

#include <mock/OctTree.hh>
#include <redoom/physics/OctTree.hh>

using redoom::physics::OctTree;

TEST_CASE("[OctTree] Basic tests", "[Physics][OctTree]")
{
  auto tree = OctTree<mock::Item, mock::ItemProxy>{{}};
  auto item_size_2 = mock::Item{glm::vec3{0.0f}, glm::vec3{2.0f}};
  auto item_size_10 = mock::Item{glm::vec3{0.0f}, glm::vec3{10.0f}};
  auto item_size_20 = mock::Item{glm::vec3{0.0f}, glm::vec3{20.0f}};

  SECTION("Single insert")
  {
    CHECK(tree.insert(item_size_20));
    CHECK(tree.getSize() == 1);
  }

  SECTION("Size-ordered multiple inserts")
  {
    CHECK(tree.insert(item_size_20));
    CHECK(tree.insert(item_size_10));
    CHECK(tree.insert(item_size_2));
    CHECK(tree.getSize() == 3);
  }

  SECTION("Reverse size-ordered multiple inserts")
  {
    CHECK(tree.insert(item_size_2));
    CHECK(tree.insert(item_size_10));
    CHECK(tree.insert(item_size_20));
    CHECK(tree.getSize() == 3);
  }

  SECTION("Single insert, bounding box contains item")
  {
    CHECK(tree.insert(item_size_20));
    REQUIRE(tree.getBoundingBox().has_value());
    CHECK(tree.getBoundingBox()->contains(item_size_20.bounding_box));
  }

  SECTION("Size-ordered multiple inserts, bounding box contains items")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    REQUIRE(tree.getBoundingBox().has_value());
    CHECK(tree.getBoundingBox()->contains(item_size_20.bounding_box));
    CHECK(tree.getBoundingBox()->contains(item_size_10.bounding_box));
    CHECK(tree.getBoundingBox()->contains(item_size_2.bounding_box));
  }

  SECTION("Size-ordered multiple inserts, bounding box contains items")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    REQUIRE(tree.getBoundingBox().has_value());
    CHECK(tree.getBoundingBox()->contains(item_size_20.bounding_box));
    CHECK(tree.getBoundingBox()->contains(item_size_10.bounding_box));
    CHECK(tree.getBoundingBox()->contains(item_size_2.bounding_box));
  }

  SECTION("Single insert and removal")
  {
    REQUIRE(tree.insert(item_size_20));
    CHECK(tree.getSize() == 1);
    CHECK(tree.remove(item_size_20));
    CHECK(tree.getSize() == 0);
  }

  SECTION("Ordered multiple inserts and removals")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    CHECK(tree.getSize() == 3);
    CHECK(tree.remove(item_size_20));
    CHECK(tree.remove(item_size_10));
    CHECK(tree.remove(item_size_2));
    CHECK(tree.getSize() == 0);
  }

  SECTION("Reverse ordered multiple inserts and removals")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    CHECK(tree.getSize() == 3);
    CHECK(tree.remove(item_size_2));
    CHECK(tree.remove(item_size_10));
    CHECK(tree.remove(item_size_20));
    CHECK(tree.getSize() == 0);
  }

  SECTION("Update tree within bounds")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    REQUIRE(tree.getBoundingBox());
    auto bounds = tree.getBoundingBox().value();
    item_size_2.bounding_box += glm::vec3{1.0f};
    CHECK(tree.update({item_size_2}));
    CHECK(tree.getBoundingBox() == bounds);
  }

  SECTION("Update tree outside bounds")
  {
    REQUIRE(tree.insert(item_size_20));
    REQUIRE(tree.insert(item_size_10));
    REQUIRE(tree.insert(item_size_2));
    REQUIRE(tree.getBoundingBox());
    auto bounds = tree.getBoundingBox().value();
    item_size_2.bounding_box += bounds.upper_bounds - bounds.lower_bounds;
    CHECK(tree.update({item_size_2}));
    CHECK(tree.getBoundingBox() != bounds);
  }
}

TEST_CASE("[OctTree] Tests with fixed size OctTree", "[Physics][OctTree]")
{
  auto bounds = redoom::physics::AABB{glm::vec3{0.0f}, glm::vec3{10.0f}};
  auto tree = OctTree<mock::Item, mock::ItemProxy>{{.bounding_box = bounds}};
  auto item_size_10 = mock::Item{glm::vec3{0.0f}, glm::vec3{10.0f}};
  auto item_size_20 = mock::Item{glm::vec3{0.0f}, glm::vec3{20.0f}};

  SECTION("Inserts within tree bounds")
  {
    REQUIRE(tree.insert(item_size_10));
    CHECK(tree.getBoundingBox() == bounds);
  }

  SECTION("Inserts outside tree bounds")
  {
    REQUIRE(tree.insert(item_size_20));
    CHECK(tree.getBoundingBox() != bounds);
  }
}
