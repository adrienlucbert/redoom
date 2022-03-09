#pragma once

#include <array>
#include <cstddef>
#include <iostream>
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

  explicit OctTree(OctTreeDefinition def = {}) noexcept
    : root_{Node{def.min_leaf_size, tl::nullopt, def.bounding_box}}
    , min_leaf_size_{def.min_leaf_size}
  {
    if (!def.items.empty())
      this->build(std::move(def.items));
  }
  ~OctTree() = default;

  OctTree(OctTree const& rhs) noexcept = delete;
  OctTree(OctTree&& rhs) noexcept = default;

  OctTree& operator=(OctTree const& rhs) noexcept = delete;
  OctTree& operator=(OctTree&& rhs) noexcept = default;

  void dump() const noexcept
  {
    if (this->root_.has_value())
      this->root_.value().dump();
  }

  void debugDraw(graphics::Program& program) const noexcept
  {
    if (this->all_items_.size() == 0)
      return;
    if (this->root_.has_value())
      this->root_->draw(program);
  }

  bool insert(T& item) noexcept
  {
    auto& ref = this->all_items_.emplace_back(std::ref(item));
    if (this->root_.has_value() && this->root_->insert(ref))
      return true;
    auto items = std::exchange(this->all_items_, {});
    return this->build(std::move(items));
  }

  bool remove(T const& item) noexcept
  {
    if (!this->root_.has_value())
      return false;
    auto it = std::find_if(this->all_items_.begin(),
        this->all_items_.end(),
        [&item](std::reference_wrapper<T> const& rhs) {
          return ItemProxy::equal(item, rhs.get());
        });
    if (it != this->all_items_.end()) {
      assert(this->root_->remove(item));
      this->all_items_.erase(it);
      return true;
    }
    return false;
  }

  bool update(std::vector<std::reference_wrapper<T>> moved_items) noexcept
  {
    if (this->root_.has_value() && this->root_->update(moved_items))
      return true;
    // update failed: some items are out of bounds
    this->build(std::exchange(this->all_items_, {}));
    return this->root_->update(moved_items);
  }

  std::vector<std::pair<T&, T&>> getClosePairs() noexcept
  {
    return this->root_->getClosePairs();
  }

  [[nodiscard]] tl::optional<AABB const&> getBoundingBox() const noexcept
  {
    if (this->root_.has_value())
      return this->root_->getBoundingBox();
    return tl::nullopt;
  }

  [[nodiscard]] std::size_t getSize() const noexcept
  {
    return this->all_items_.size();
  }

private:
  class Node
  {
  public:
    explicit Node(float min_leaf_size,
        tl::optional<std::reference_wrapper<Node>> parent,
        AABB bounding_box) noexcept
      : min_leaf_size_{min_leaf_size}
      , parent_{parent}
      , bounding_box_{bounding_box}
    {
    }
    ~Node() noexcept = default;

    Node(Node const&) noexcept = default;
    Node(Node&&) noexcept = default;

    Node& operator=(Node const&) noexcept = default;
    Node& operator=(Node&&) noexcept = default;

    void dump(std::size_t indent = 0u) const noexcept
    {
      auto const& aabb = this->getBoundingBox();
      this->out(indent) << fmt::format("/{: .2f} {: .2f} {: .2f}\\\n",
          aabb.lower_bounds.x,
          aabb.lower_bounds.y,
          aabb.lower_bounds.z);
      this->out(indent) << fmt::format("\\{: .2f} {: .2f} {: .2f}/\n",
          aabb.upper_bounds.x,
          aabb.upper_bounds.y,
          aabb.upper_bounds.z);
      this->out(indent) << fmt::format("size: {}", this->size_) << std::endl;
      this->out(indent) << fmt::format("items: {}", this->items_.size())
                        << std::endl;
      for (auto const& item : this->items_) {
        auto const& iaabb = ItemProxy::getAABB(item);
        this->out(indent + 1) << std::addressof(item.get()) << std::endl;
        this->out(indent + 1) << fmt::format("/{: .2f} {: .2f} {: .2f}\\\n",
            iaabb.lower_bounds.x,
            iaabb.lower_bounds.y,
            iaabb.lower_bounds.z);
        this->out(indent + 1) << fmt::format("\\{: .2f} {: .2f} {: .2f}/\n",
            iaabb.upper_bounds.x,
            iaabb.upper_bounds.y,
            iaabb.upper_bounds.z);
        this->out(indent + 1) << std::endl;
      }
      this->out(indent) << std::endl;
      if (this->hasChildren()) {
        for (auto const& child : this->children_)
          child->dump(indent + 2);
      }
    }

    [[nodiscard]] std::ostream& out(std::size_t indent) const noexcept
    {
      return std::cout << fmt::format("{:\t>{}}", "", indent);
    }

    [[nodiscard]] AABB const& getBoundingBox() const noexcept
    {
      return this->bounding_box_;
    }

    [[nodiscard]] std::size_t getSize() const noexcept
    {
      return this->size_;
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
        auto current_opt = target_opt->get().forwardSearch(item);
        if (!current_opt.has_value())
          // item not found in the containing node or its children
          // perform backward search to search for item in the tree backward
          // until tree root
          current_opt = target_opt->get().backwardSearch(item);
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
      auto items = std::exchange(this->items_, {});
      this->clear();
      for (auto const& item : items) {
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
        if (this->parent_.has_value())
          return this->parent_->get().insert(item);
        return false;
      }

      // this leaf is empty, just add the item to it
      if (this->size_ == 0) {
        this->addItem(item);
        return true;
      }

      auto dimension = this->bounding_box_.getSize();

      if (dimension.x <= this->min_leaf_size_
          && dimension.y <= this->min_leaf_size_
          && dimension.z <= this->min_leaf_size_) {
        // this leaf's size is already the minimum size for a leaf
        // just add the item to it
        this->addItem(item);
        return true;
      }

      // item bounding box is within this leaf's bounds
      if (!this->hasChildren())
        this->split();

      // try to fit the item into a child leaf
      for (auto& child : this->children_) {
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
      auto node_opt = this->forwardSearch(item);
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
        for (auto& child : this->children_) {
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
     * Search is conducted in a up-to-bottom approach
     */
    tl::optional<std::reference_wrapper<Node>> forwardSearch(
        T const& item, int max_depth = -1)
    {
      if (max_depth == 0)
        return tl::nullopt;

      // check if item is contained in this node's items
      if (this->holdsItem(item))
        return std::ref(*this);

      if (this->hasChildren()) {
        for (auto const& child : this->children_) {
          if (child->contains(item)) {
            // item bounding box is within this child's bounds, so it might be
            // found here
            auto search_res_opt = child->forwardSearch(item, max_depth - 1);
            // if it it found here, return the result of the search
            if (search_res_opt.has_value())
              return search_res_opt;
            // otherwise, stop forward search, and try brute search
            break;
          }
        }

        // item wasn't found where it was supposed to be, perform a brute
        // search
        for (auto const& child : this->children_) {
          auto search_res_opt = child->bruteSearch(item, max_depth - 1);
          if (search_res_opt.has_value())
            return search_res_opt;
        }
      }

      // couldn't find item in this node or its children, even with brute
      // search this node or its children don't contain the item searched for
      return tl::nullopt;
    }

    /** Search in the tree the node which contains the given item
     * Search is conducted in a bottom-to-up approach
     */
    tl::optional<std::reference_wrapper<Node>> backwardSearch(
        T const& item) noexcept
    {
      // item is contained in this node
      if (this->holdsItem(item))
        return std::ref(*this);

      if (this->hasChildren()) {
        for (auto const& child : this->children_) {
          if (child->contains(item)) {
            // item bounding box is within this child's bounds, so it might be
            // found here
            auto search_res_opt = child->forwardSearch(item, 1);
            if (search_res_opt.has_value())
              return search_res_opt;
            break;
          }
        }
      }

      if (this->parent_.has_value()) {
        auto search_res_opt = this->parent_->get().backwardSearch(item);
        if (search_res_opt.has_value())
          return search_res_opt;
      }
      // item wasn't found where it was supposed to be, perform a brute search
      return this->bruteSearch(item);
    }

    /** Search in every leaf of the tree the node which contains the given
     * item
     */
    tl::optional<std::reference_wrapper<Node>> bruteSearch(
        T const& item, int max_depth = -1) noexcept
    {
      if (max_depth == 0)
        return tl::nullopt;

      // item is contained in this node
      if (this->holdsItem(item))
        return *this;

      if (this->hasChildren()) {
        for (auto const& child : this->children_) {
          auto search_res_opt = child->bruteSearch(item, max_depth - 1);
          if (search_res_opt.has_value())
            // item bounding box is within this child's bounds
            return search_res_opt;
        }
      }
      // item wasn't found in this node or its children, return null
      return tl::nullopt;
    }

    /** Returns an iterator to the given item in this node's items, or a
     * past-the-end iterator if not found
     */
    [[nodiscard]] auto findItem(T const& item) const noexcept
    {
      return std::find_if(this->items_.begin(),
          this->items_.end(),
          [&item](std::reference_wrapper<T> const& rhs) {
            return ItemProxy::equal(item, rhs.get());
          });
    }

    /** Check if the given item is contained in this node's direct items
     * (doesn't check children)
     */
    [[nodiscard]] bool holdsItem(T const& item) const noexcept
    {
      auto it = this->findItem(item);
      return it != this->items_.end();
    }

    std::vector<std::pair<T&, T&>> getClosePairs() noexcept
    {
      auto pairs = std::vector<std::pair<T&, T&>>{};
      if (this->size_ <= 1)
        return pairs;

      // make pairs with this node's items
      for (auto it = this->items_.begin(); it != this->items_.end(); ++it) {
        auto own_pairs = this->makePairsWith(it);
        pairs.reserve(pairs.size() + own_pairs.size());
        std::move(
            own_pairs.begin(), own_pairs.end(), std::back_inserter(pairs));
      }

      if (this->hasChildren()) {
        for (auto& child : this->children_) {
          auto child_pairs = child->getClosePairs();
          pairs.reserve(pairs.size() + child_pairs.size());
          std::move(child_pairs.begin(),
              child_pairs.end(),
              std::back_inserter(pairs));
        }
      }
      return pairs;
    }

    std::vector<std::pair<T&, T&>> makePairsWith(
        typename std::list<std::reference_wrapper<T>>::iterator
            item_it) noexcept
    {
      auto pairs = std::vector<std::pair<T&, T&>>{};

      for (auto it = std::next(item_it); it != this->items_.end(); ++it)
        pairs.emplace_back(*item_it, *it);

      if (this->hasChildren()) {
        for (auto const& child : this->children_) {
          auto child_pairs = child->makePairsWith(*item_it);
          pairs.reserve(pairs.size() + child_pairs.size());
          std::move(child_pairs.begin(),
              child_pairs.end(),
              std::back_inserter(pairs));
        }
      }
      return pairs;
    }

    std::vector<std::pair<T&, T&>> makePairsWith(T& item) noexcept
    {
      auto pairs = std::vector<std::pair<T&, T&>>{};

      for (auto& it : this->items_)
        pairs.emplace_back(item, it);

      if (this->hasChildren()) {
        for (auto const& child : this->children_) {
          auto child_pairs = child->makePairsWith(item);
          pairs.reserve(pairs.size() + child_pairs.size());
          std::move(child_pairs.begin(),
              child_pairs.end(),
              std::back_inserter(pairs));
        }
      }
      return pairs;
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
        for (auto const& child : this->children_)
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
      return this->children_[0] != nullptr;
    }

    void clear() noexcept
    {
      this->items_.clear();
      this->updateSize(-static_cast<int>(this->size_));
    }

    /** Check if this node's bounding box contains the given item's bounding
     * box
     */
    bool contains(T const& item) const noexcept
    {
      return this->bounding_box_.contains(ItemProxy::getAABB(item));
    }

    /** Increment or decrement the tree size (and its parent's recursively)
     */
    void updateSize(int add) noexcept
    {
      if (add < 0)
        this->size_ -= static_cast<std::size_t>(-add);
      else
        this->size_ += static_cast<std::size_t>(add);
      if (this->parent_.has_value())
        this->parent_->get().updateSize(add);
    }

    /** Add an item to the current node and update the tree size
     */
    void addItem(std::reference_wrapper<T> item) noexcept
    {
      this->items_.emplace_back(item);
      this->updateSize(+1);
    }

    /** Remove an item from the current node and update the tree size
     */
    bool removeItem(T const& item) noexcept
    {
      auto it = this->findItem(item);
      if (it != this->items_.end()) {
        this->items_.erase(it);
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
      auto const lb = this->bounding_box_.lower_bounds;
      auto const ub = this->bounding_box_.upper_bounds;
      auto const center = this->bounding_box_.getCenter();
      this->children_ = {this->createChild({{center.x, lb.y, lb.z},
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
    std::unique_ptr<Node> createChild(AABB bounding_box) noexcept
    {
      return std::make_unique<Node>(this->min_leaf_size_, *this, bounding_box);
    }

    float min_leaf_size_;
    tl::optional<std::reference_wrapper<Node>> parent_{};
    AABB bounding_box_;
    std::array<std::unique_ptr<Node>, 8> children_{};
    std::size_t size_{0};
    std::list<std::reference_wrapper<T>> items_{};

    friend class OctTree<T, ItemProxy>;
  };

  bool build(std::list<std::reference_wrapper<T>> items) noexcept
  {
    if (items.empty())
      return true;

    auto aabb = tl::optional<AABB>{tl::nullopt};
    for (auto const& item : items) {
      if (aabb.has_value())
        aabb = aabb->combine(ItemProxy::getAABB(item));
      else
        aabb = AABB{ItemProxy::getAABB(item)};
    }
    assert(aabb.has_value());
    this->root_ = Node{this->min_leaf_size_, tl::nullopt, *aabb};
    for (auto const& item : items) {
      if (!this->insert(item))
        return false;
    }
    return true;
  }

  tl::optional<Node> root_;
  std::list<std::reference_wrapper<T>> all_items_{};
  float min_leaf_size_;

  friend class Node;
};
} // namespace redoom::physics
