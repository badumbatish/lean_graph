#pragma once
#include <__expected/unexpected.h>
#include <algorithm>
#include <expected>
#include <functional>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

enum class node_error { not_exist, duplicate, general_error };
enum class edge_error { not_exist, duplicate, general_error };
using CounterType = uint32_t;

/// INFO: A counter class
template <class Aspect> class Counter {
  std::unordered_map<Aspect, CounterType> counter;
  CounterType count;

public:
  Counter(CounterType start_from = 0) : count(start_from) {}
  bool exist(Aspect aspect) const {
    return counter.find(aspect) != counter.end();
  }
  CounterType get_counter(Aspect aspect) {
    if (!exist(aspect))
      counter[aspect] = count++;
    return counter[aspect];
  }

  bool counter_exceeds(CounterType ceiling) const { return count > ceiling; }
};

/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class Cost, class CounterType = std::uint16_t>
  requires std::is_arithmetic_v<Cost>
class DiGraph {
public:
  //  INFO: Edges type used internally and the user uses to interface with the
  //  graph
  using CounterEdge = std::tuple<CounterType, CounterType, Cost>;
  using CounterBlankEdge = std::tuple<CounterType, CounterType>;
  using CounterHalfEdge = std::tuple<CounterType, Cost>;

  // INFO: Edge that the user use to register an edge
  using Edge = std::tuple<CounterType, CounterType, Cost>;
  using HalfEdge = std::tuple<CounterType, Cost>;

  //  INFO: Results
  using NodeResult = std::expected<CounterType, node_error>;
  using EdgeResult = std::expected<CounterEdge, node_error>;

protected:
  enum VisitOrder { pre, post };
  std::map<CounterType, std::set<CounterHalfEdge>> graph;
  Counter<CounterType> node_counter;
  auto NodeToCounter(CounterType node) -> NodeResult {
    if (!node_counter.exist(node))
      return std::unexpected<node_error>(node_error::not_exist);
    return node_counter.get_counter(node);
  }

public:
  auto registerNode(CounterType node) -> CounterType {
    if (existNode(node))
      return node_counter.get_counter(node);

    return node_counter.get_counter(node);
  }

  auto registerEdge(CounterEdge edge) -> CounterEdge {
    if (existEdge(edge))
      return edge;

    const auto &[from, to, cost] = edge;
    this->graph[from].insert({to, cost});
    return edge;
  }

  auto existEdge(CounterEdge edge) const -> bool {
    auto &[from, to, cost] = edge;
    if (!existNode(from))
      return false;
    auto s = this->graph.find(from);
    if (s == this->graph.end())
      return false;

    return (*s).second.contains({to, cost});
  }
  auto existBlankEdge(CounterBlankEdge edge) const -> bool {
    /// NOTE: To bypass unused var warning in structured bindings
    auto from = std::get<0>(edge), to = std::get<1>(edge);
    /*auto [from, to] = edge;*/
    if (!existNode(from))
      return false;
    auto s = this->graph.find(from);
    if (s == this->graph.end())
      return false;

    auto st = (*s).second;
    return std::find_if(st.begin(), st.end(), [=](const auto &p) {
             return to == std::get<0>(p);
           }) != st.end();
  }
  auto existNode(CounterType node) const -> bool {
    return node_counter.exist(node);
  }

  /// INFO: Performs dfs with 2 callables of pre, and post order
  auto dfs(CounterType from, std::function<bool(CounterType)> pre,
           std::function<bool(CounterType)> post) const
      -> std::optional<node_error> {
    using tup = std::tuple<CounterType, VisitOrder>;
    std::stack<tup> stck;
    std::unordered_set<CounterType> visited;
    if (!existNode(from))
      return node_error::not_exist;

    stck.push(tup(from, VisitOrder::pre));
    while (!stck.empty()) {
      auto [current_node, visit_order] = stck.top();
      stck.pop();
      visited.insert(current_node);

      if (visit_order == VisitOrder::pre) {
        if (pre && !pre(current_node))
          return std::nullopt;

        stck.push(tup(current_node, VisitOrder::post));
        auto neighbors = graph.find(current_node);
        if (neighbors == graph.end())
          continue;
        for (auto &[neighbor, cost] : (*neighbors).second) {
          if (visited.contains(neighbor))
            continue;
          stck.push(tup(neighbor, VisitOrder::pre));
        }
      } else {
        if (post && !post(current_node))
          return std::nullopt;
      }
    }

    return std::nullopt;
  }
  /// INFO: Performs dfs with 2 callables of pre, and post order
  auto bfs(CounterType from, std::function<bool(CounterType)> pre,
           std::function<bool(CounterType)> post) const
      -> std::optional<node_error> {
    using tup = std::tuple<CounterType, VisitOrder>;
    std::queue<tup> q;
    std::unordered_set<CounterType> visited;

    // NOTE: Check if the node actually exists
    if (!existNode(from))
      return node_error::not_exist;

    q.push(tup(from, VisitOrder::pre));

    while (!q.empty()) {
      auto [current_node, visit_order] = q.front();
      q.pop();
      visited.insert(current_node);

      if (visit_order == VisitOrder::pre) {
        if (pre && !pre(current_node))
          return std::nullopt;

        q.push(tup(current_node, VisitOrder::post));
        auto neighbors = graph.find(current_node);
        if (neighbors == graph.end())
          continue;
        for (auto &[neighbor, cost] : (*neighbors).second) {
          if (visited.contains(neighbor))
            continue;
          q.push(tup(neighbor, VisitOrder::pre));
        }
      } else {
        if (post && !post(current_node))
          return std::nullopt;
      }
    }

    return std::nullopt;
  }

  /// INFO: Single source, single path dijkstra algorithm
  /// User discretion required, user might input negative cost.
  auto djikstra(CounterType from, CounterType to) const
      -> std::vector<CounterType>;
};

/// INFO: A DAG is a Directed Acyclic Graph that can be multi-edges, with
/// edge-cost being different.
template <class Cost, class CounterType = std::uint16_t>
  requires std::is_arithmetic_v<Cost>
class DAG : public DiGraph<Cost, CounterType> {
public:
  // A topological sort is a reversed post order
  auto topo_sort() -> std::vector<CounterType> const {

    std::vector<CounterType> result;
    auto vec_push = [&](auto node) {
      result.push_back(node);
      return true;
    };

    auto from = this->graph.begin();
    if (from == this->graph.end())
      return {};

    auto dfs_result = dfs((*this->graph.begin()).first, nullptr, vec_push);
    std::ranges::reverse(result);
    if (dfs_result == std::nullopt)
      return result;
    return {};
  }
};

/*template class DiGraph<uint32_t, uint32_t>;*/
/*template class DiGraph<float_t>;*/
/*template <class T, class C> class UGraph : public BasicGraph<T, C> {};*/
/*template <class T, class C> class DiGraph : public BasicGraph<T, C> {};*/
