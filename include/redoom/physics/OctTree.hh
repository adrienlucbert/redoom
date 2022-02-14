#pragma once

#include <array>
#include <cstddef>
#include <list>
#include <memory>

#include <fmt/format.h>
#include <tl/optional.hpp>

#include <redoom/graphics/Program.hh>
#include <redoom/graphics/mesh/Cuboid.hh>
#include <redoom/physics/AABB.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::physics
{
namespace concepts
{
// clang-format off
template <typename ProxyT, typename T>
concept OctTreeItemProxy = requires(T const&item, T const&rhs)
{
  { ProxyT::getAABB(item) } noexcept -> std::same_as<AABB>;
  { ProxyT::equal(item, rhs) } noexcept -> std::same_as<bool>;
};
// clang-format on
} // namespace concepts

template <typename T>
struct BaseOctTreeItemProxy {
  [[nodiscard]] static bool equal(T const& lhs, T const& rhs) noexcept
  {
    return std::addressof(lhs) == std::addressof(rhs);
  }
};

template <typename T, concepts::OctTreeItemProxy<T> ItemProxy>
class OctTree
{
public:
  struct OctTreeDefinition {
    AABB bounding_box{};
    float min_leaf_size{1.0f};
    std::list<std::reference_wrapper<T>> items{};
  };

  explicit OctTree(OctTreeDefinition def) noexcept
    : root{tl::nullopt}
    , min_leaf_size{def.min_leaf_size}
  {
    if (!def.items.empty())
      this->build(std::move(all_items));
  }

  void debugDraw(graphics::Program& program) const noexcept
  {
    if (this->root.has_value())
      this->root->draw(program);
  }

  bool insert(T& item) noexcept
  {
    auto& ref = this->all_items.emplace_back(std::ref(item));
    if (this->root.has_value() && this->root->insert(ref))
      return true;
    auto pitems = std::move(this->all_items);
    this->all_items.clear();
    return this->build(std::move(pitems));
  }

  bool remove(T const& item) noexcept
  {
    if (!this->root.has_value())
      return false;
    auto it = std::find_if(this->all_items.begin(),
        this->all_items.end(),
        [&item](T const& rhs) { return ItemProxy::equal(item, rhs); });
    if (it != this->all_items.end()) {
      assert(this->root->remove(item));
      this->all_items.erase(it);
      return true;
    }
    return false;
  }

  bool update(std::vector<std::reference_wrapper<T>> moved_items) noexcept
  {
    if (this->root.has_value() && this->root->update(moved_items))
      return true;
    // update failed: some items are out of bounds
    this->build(std::move(this->all_items));
    return this->root->update(moved_items);
  }

  [[nodiscard]] tl::optional<AABB const&> getBoundingBox() const noexcept
  {
    if (this->root.has_value())
      return this->root->getBoundingBox();
    return tl::nullopt;
  }

  [[nodiscard]] std::size_t getSize() const noexcept
  {
    return this->all_items.size();
  }

private:
  class Node
  {
  public:
    explicit Node(OctTree<T, ItemProxy>& ptree,
        tl::optional<Node&> pparent,
        AABB pbounding_box) noexcept
      : tree{ptree}
      , parent{pparent}
      , bounding_box{pbounding_box}
    {
    }
    ~Node() noexcept = default;

    Node(Node const&) noexcept = default;
    Node(Node&&) noexcept = default;

    Node& operator=(Node const&) noexcept = default;
    Node& operator=(Node&&) noexcept = default;

    [[nodiscard]] AABB const& getBoundingBox() const noexcept
    {
      return this->bounding_box;
    }

    [[nodiscard]] std::size_t getSize() const noexcept
    {
      return this->size;
    }

    /** Update items positions in the tree
     */
    bool update(std::vector<std::reference_wrapper<T>> moved_items) noexcept
    {
      if (moved_items.empty())
        // no moved items, nothing to do
        return true;

      for (auto const& item : moved_items) {
        // find where the item should be
        auto target_opt = this->findContainingNode(item);
        if (!target_opt.has_value())
          // item could not fit in this node
          return false;

        // find where the item currently is
        auto current_opt = this->search(item);
        assert(current_opt.has_value() && "item is not contained in this tree");

        if (std::addressof(*target_opt) != std::addressof(*current_opt)) {
          // item is not where it should be, move it
          current_opt->get().remove(item);
          if (!target_opt->get().insert(item)) {
            // insertion failed: item is out of bounds for this tree
            return false;
          }
        }
      }
      return true;
    }

    /** Update the position of all items in this node, trying to fit them into
     * this node's children if possible
     */
    bool updateAll() noexcept
    {
      auto pitems = std::move(this->items);
      this->clear();
      for (auto const& item : pitems) {
        if (!this->insert(item))
          return false;
      }
      return true;
    }

    /** Insert an item in the tree
     */
    bool insert(std::reference_wrapper<T> item) noexcept
    {
      if (!this->contains(item.get())) {
        // item bounding box is out of this leaf's bounds
        if (this->parent.has_value())
          return this->parent->insert(item);
        return false;
      }

      // this leaf is empty, just add the item to it
      if (this->size == 0) {
        this->addItem(item);
        return true;
      }

      auto dimension = this->bounding_box.getSize();

      if (dimension.x <= this->tree.get().min_leaf_size
          && dimension.y <= this->tree.get().min_leaf_size
          && dimension.z <= this->tree.get().min_leaf_size) {
        // this leaf's size is already the minimum size for a leaf
        // just add the item to it
        this->addItem(item);
        return true;
      }

      // item bounding box is within this leaf's bounds
      if (!this->hasChildren())
        this->split();

      // try to fit the item into a child leaf
      for (auto& child : this->children) {
        // item bounding box is within this child's bounds
        if (child->contains(item.get()))
          return child->insert(item);
      }
      // item bounding box is within none of the children bounds
      // item can't fit in a smaller leaf, then add it to this one
      this->addItem(item);
      return true;
    }

    /** Remove an item from the tree
     */
    bool remove(T const& item) noexcept
    {
      auto node_opt = this->search(item);
      if (!node_opt.has_value())
        return false;
      return node_opt->get().removeItem(item);
    }

    /** Search in the tree the node which should contain the given item
     * The item is not necessarily contained in this node, but it could fit in
     * it
     */
    tl::optional<std::reference_wrapper<Node>> findContainingNode(
        T const& item) noexcept
    {
      // item bounding box is not within this leaf's bounds
      if (!this->contains(item))
        return tl::nullopt;

      // search within this leaf's children if any
      if (this->hasChildren()) {
        for (auto& child : this->children) {
          auto search_res_opt = child->findContainingNode(item);
          if (search_res_opt.has_value())
            // item bounding box is within this child's bounds
            return search_res_opt;
        }
      }
      // item wasn't found in a smaller leaf, so this is the containing leaf
      return std::ref(*this);
    }

    /** Search in the tree the node which contains the given item
     */
    tl::optional<std::reference_wrapper<Node>> search(T const& item) noexcept
    {
      // item is contained in this node
      auto it = std::find_if(this->items.begin(),
          this->items.end(),
          [&item](T const& rhs) { return ItemProxy::equal(item, rhs); });
      if (it != this->items.end())
        return std::ref(*this);

      if (this->hasChildren()) {
        for (auto const& child : this->children) {
          if (child->contains(item))
            // item bounding box is within this child's bounds, so it might be
            // found here
            return child->search(item);
        }
      }
      // item wasn't found where it was supposed to be, perform a brute search
      return this->bruteSearch(item);
    }

    /** Search in every leaf of the tree the node which contains the given
     * item
     */
    tl::optional<std::reference_wrapper<Node>> bruteSearch(
        T const& item) noexcept
    {
      // item is contained in this node
      auto it = std::find_if(this->items.begin(),
          this->items.end(),
          [&item](T const& rhs) { return ItemProxy::equal(item, rhs); });
      if (it != this->items.end())
        return std::ref(*this);

      if (this->hasChildren()) {
        for (auto const& child : this->children) {
          auto search_res_opt = child->search(item);
          if (search_res_opt.has_value())
            // item bounding box is within this child's bounds
            return search_res_opt;
        }
      }
      // item wasn't found in this node or its children, return null
      return tl::nullopt;
    }

    void draw(graphics::Program& program) const noexcept
    {
      auto const& aabb = this->getBoundingBox();
      auto offset = aabb.getCenter();
      auto model = glm::mat4{1.0f};
      model = glm::translate(model, offset);
      model = glm::scale(model, aabb.getSize());
      renderer::Renderer::draw(program, *this->getMesh(), model);

      if (this->hasChildren()) {
        for (auto const& child : this->children)
          child->draw(program);
      }
    }

  private:
    static std::unique_ptr<graphics::Mesh const> const& getMesh() noexcept
    {
      static auto const mesh =
          std::make_unique<graphics::Mesh const>(graphics::mesh::Cuboid{
              1.0f, 1.0f, 1.0f, {0.0f, 0.0f, 1.0f}, {}, GL_LINE_STRIP});
      return mesh;
    }

    /** Check if this node has children
     */
    [[nodiscard]] bool hasChildren() const noexcept
    {
      return this->children[0] != nullptr;
    }

    void clear() noexcept
    {
      this->items.clear();
      this->updateSize(-static_cast<int>(this->size));
    }

    /** Check if this node's bounding box contains the given item's bounding box
     */
    bool contains(T const& item) const noexcept
    {
      return this->bounding_box.contains(ItemProxy::getAABB(item));
    }

    /** Increment or decrement the tree size (and its parent's recursively)
     */
    void updateSize(int add) noexcept
    {
      if (add < 0)
        this->size -= static_cast<std::size_t>(-add);
      else
        this->size += static_cast<std::size_t>(add);
      if (this->parent.has_value())
        this->parent->updateSize(add);
    }

    /** Add an item to the current node and update the tree size
     */
    void addItem(std::reference_wrapper<T> item) noexcept
    {
      this->items.emplace_back(item);
      this->updateSize(+1);
    }

    /** Remove an item from the current node and update the tree size
     */
    bool removeItem(T const& item) noexcept
    {
      auto it = std::find_if(this->items.begin(),
          this->items.end(),
          [&item](T const& rhs) { return ItemProxy::equal(item, rhs); });
      if (it != this->items.end()) {
        this->items.erase(it);
        this->updateSize(-1);
        return true;
      }
      return false;
    }

    /** Split the current node into 8 sub nodes of equal size
     */
    void split() noexcept
    {
      assert(this->hasChildren() == false && "Tree is already split");
      auto const lb = this->bounding_box.lower_bounds;
      auto const ub = this->bounding_box.upper_bounds;
      auto const center = this->bounding_box.getCenter();
      this->children = {this->createChild({{center.x, lb.y, lb.z},
                            {ub.x, center.y, center.z}}),
          this->createChild({lb, center}),
          this->createChild(
              {{lb.x, lb.y, center.z}, {center.x, center.y, ub.z}}),
          this->createChild(
              {{center.x, lb.y, center.z}, {ub.x, center.y, ub.z}}),
          this->createChild(
              {{center.x, center.y, lb.z}, {ub.x, ub.y, center.z}}),
          this->createChild(
              {{lb.x, center.y, lb.z}, {center.x, ub.y, center.z}}),
          this->createChild(
              {{lb.x, center.y, center.z}, {center.x, ub.y, ub.z}}),
          this->createChild({center, ub})};
      // redistribute this node's items in the newly-created children
      this->updateAll();
    }

    /** Create a child to the current node given its bounding box
     */
    std::unique_ptr<Node> createChild(AABB pbounding_box) noexcept
    {
      return std::make_unique<Node>(this->tree, *this, pbounding_box);
    }

    std::reference_wrapper<OctTree<T, ItemProxy>> tree;
    tl::optional<Node&> parent{};
    AABB bounding_box;
    std::array<std::unique_ptr<Node>, 8> children{};
    std::size_t size{0};
    std::list<std::reference_wrapper<T>> items{};

    friend class OctTree<T, ItemProxy>;
  };

  bool build(std::list<std::reference_wrapper<T>> pitems) noexcept
  {
    if (pitems.empty())
      return true;

    auto aabb = tl::optional<AABB>{tl::nullopt};
    for (auto const& item : pitems) {
      if (aabb.has_value())
        aabb = aabb->combine(ItemProxy::getAABB(item));
      else
        aabb = AABB{ItemProxy::getAABB(item)};
    }
    assert(aabb.has_value());
    this->root = Node{*this, tl::nullopt, *aabb};
    for (auto const& item : pitems) {
      if (!this->insert(item))
        return false;
    }
    return true;
  }

  tl::optional<Node> root;
  std::list<std::reference_wrapper<T>> all_items{};
  float min_leaf_size;

  friend class Node;
};
} // namespace redoom::physics
