#pragma once
#include <__expected/unexpected.h>
#include <algorithm>
#include <cmath>
#include <expected>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace lean_graph {

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// Declaration of the concept “Hashable”, which is satisfied by any type “T”
// such that for values “a” of type “T”, the expression std::hash<T>{}(a)
// compiles and its result is convertible to std::size_t
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

enum class node_error { not_exist, duplicate, general_error };
enum class edge_error { not_exist, duplicate, general_error };
enum VisitOrder { pre, post };
/// INFO: A counter class
template <class Aspect, class CounterType> class Counter {
  std::unordered_map<Aspect, CounterType> counter;
  CounterType count;

public:
  Counter(CounterType start_from = 0) : count(start_from) {}
  bool exist(Aspect aspect) const {
    return counter.find(aspect) != counter.end();
  }
  bool counter_exceeds(CounterType aspect) const { return count > aspect; }
  CounterType get_counter(Aspect aspect) {
    if (!exist(aspect))
      counter[aspect] = count++;
    return counter[aspect];
  }
};

/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
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
  std::unordered_map<CounterType, std::set<CounterHalfEdge>> graph;
  Counter<NodeType, CounterType> node_counter;

  template <VisitOrder v>
  auto explore_dfs_protected(
      CounterType from,
      std::optional<std::reference_wrapper<std::unordered_set<CounterType>>>
          pre_visited = std::nullopt) const -> std::vector<CounterType> {
    using tup = std::tuple<CounterType, VisitOrder>;
    std::stack<tup> stck;
    std::unordered_set<CounterType> local_visited;
    std::unordered_set<CounterType> &visited =
        pre_visited.has_value() ? pre_visited->get() : local_visited;
    std::vector<CounterType> result;
    if (!existCounterNode(from))
      return {};

    stck.push(tup(from, VisitOrder::pre));
    while (!stck.empty()) {
      auto [current_node, visit_order] = stck.top();
      stck.pop();
      visited.insert(current_node);

      if (visit_order == VisitOrder::pre) {
        if constexpr (v == VisitOrder::pre)
          result.push_back(current_node);

        stck.push(tup(current_node, VisitOrder::post));
        auto neighbors = graph.find(current_node);
        if (neighbors == graph.end())
          continue;
        for (auto &[neighbor, cost] : (*neighbors).second) {
          if (visited.contains(neighbor))
            continue;
          stck.push(tup(neighbor, VisitOrder::pre));
        }
      } else if constexpr (v == VisitOrder::post)
        result.push_back(current_node);
    }

    return result;
  }

  template <VisitOrder v>
  auto explore_bfs_protected(
      CounterType from,
      std::optional<std::reference_wrapper<std::unordered_set<CounterType>>>
          pre_visited = std::nullopt) const -> std::vector<CounterType> {
    using tup = std::tuple<CounterType, VisitOrder>;
    std::queue<tup> q;
    std::unordered_set<CounterType> local_visited;
    std::unordered_set<CounterType> &visited =
        pre_visited.has_value() ? pre_visited->get() : local_visited;
    std::vector<CounterType> result;
    if (!existNode(from))
      return {};

    q.push(tup(from, VisitOrder::pre));
    while (!q.empty()) {
      auto [current_node, visit_order] = q.front();
      q.pop();
      visited.insert(current_node);

      if (visit_order == VisitOrder::pre) {
        if constexpr (v == VisitOrder::pre)
          result.push_back(current_node);

        q.push(tup(current_node, VisitOrder::post));
        auto neighbors = graph.find(current_node);
        if (neighbors == graph.end())
          continue;
        for (auto &[neighbor, cost] : (*neighbors).second) {
          if (visited.contains(neighbor))
            continue;
          q.push(tup(neighbor, VisitOrder::pre));
        }
      } else if constexpr (v == VisitOrder::post)
        result.push_back(current_node);
    }

    return result;
  }

public:
  auto registerNode(NodeType node) -> CounterType {
    if (existNode(node))
      return node_counter.get_counter(node);

    return node_counter.get_counter(node);
  }

  auto registerEdge(CounterEdge edge) -> void {
    const auto [from, to, cost] = edge;
    this->graph[from].insert({to, cost});
    return;
  }

  auto modifyEdge(CounterEdge edge, Cost new_cost)
      -> std::optional<edge_error> {
    if (!existEdge(edge))
      return edge_error::duplicate;

    const auto &[from, to, old_cost] = edge;
    this->graph[from].erase({to, old_cost});
    this->graph[from].insert({to, new_cost});
    return std::nullopt;
  }

  auto existEdge(CounterEdge edge) const -> bool {
    auto &[from, to, cost] = edge;
    if (!existCounterNode(from))
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
    if (!existCounterNode(from))
      return false;
    auto s = this->graph.find(from);
    if (s == this->graph.end())
      return false;

    auto st = (*s).second;
    return std::find_if(st.begin(), st.end(), [=](const auto &p) {
             return to == std::get<0>(p);
           }) != st.end();
  }
  auto existBlankEdge(CounterEdge edge) const -> bool {
    auto [from, to, cost] = edge;
    return existBlankEdge({from, to});
  }
  auto existNode(NodeType node) const -> bool {
    return node_counter.exist(node);
  }
  auto existCounterNode(CounterType node) const -> bool {
    return node_counter.counter_exceeds(node);
  }

  /// INFO: Performs full dfs of all nodes connected to a node
  /// with either pre or post order from a single node
  template <VisitOrder v> auto dfs() const -> std::vector<CounterType> {
    std::unordered_set<CounterType> visited;
    std::vector<CounterType> result;
    for (auto [node, st] : graph) {
      if (!visited.contains(node))
        result.insert_range(result.end(),
                            explore_dfs_protected<v>(node, visited));
    }

    return result;
  }

  /// INFO: Performs full dfs of all nodes connected to a node
  /// with either pre or post order from a single node
  template <VisitOrder v> auto bfs() const -> std::vector<CounterType> {
    std::unordered_set<CounterType> visited;
    std::vector<CounterType> result;
    for (auto [node, st] : graph) {
      if (!visited.contains(node))
        result.insert_range(result.end(),
                            explore_bfs_protected<v>(node, visited));
    }
    return result;
  }

  /// INFO: Performs exploration of all nodes connected to a node in dfs fashion
  /// with either pre or post order from a single node
  template <VisitOrder v>
  auto explore_dfs(CounterType from) const -> std::vector<CounterType> {
    return explore_dfs_protected<v>(from, std::nullopt);
  }

  /// INFO: Performs exploration of all nodes connected to a node in bfs fashion
  /// with either pre or post order as template from a single node
  template <VisitOrder v>
  auto explore_bfs(CounterType from) const -> std::vector<CounterType> {
    return explore_bfs_protected<v>(from, std::nullopt);
  }

  /// INFO: Single source, single path dijkstra algorithm
  /// User discretion required, user might input negative cost.
  ///
  /// Successful djikstra will contain at least a vector of two nodes.
  /// If you're not a nerd, please be careful
  auto djikstra(CounterType start, CounterType end,
                auto compare = std::greater<>())
      -> std::tuple<Cost, std::vector<CounterType>> {
    if (!existNode(start) || !existNode(end))
      return {0, {}};
    std::unordered_map<CounterType, Cost> dist_from_start;
    std::unordered_map<CounterType, CounterType> prev;
    dist_from_start[start] = 0;
    std::priority_queue<std::tuple<Cost, CounterType>,
                        std::vector<std::tuple<Cost, CounterType>>,
                        decltype(compare)>
        pq(compare);

    pq.emplace(dist_from_start[start], start);
    while (!pq.empty()) {
      auto [dist_node, node] = pq.top();
      pq.pop();

      for (auto [neighbor, cost] : graph[node]) {
        if (!dist_from_start.contains(neighbor) or
            (dist_from_start[neighbor] > dist_node[node] + cost)) {
          dist_from_start[neighbor] = dist_node[node] + cost;
          prev[neighbor] = node;
          pq.emplace(dist_from_start[neighbor], neighbor);
        }
      }
    }

    if (!prev.contains(end))
      return {0, {}};

    auto p = prev[end];
    std::vector<CounterType> djikstra_path = {end};
    while (p != start) {
      djikstra_path.push_back(p);
      p = prev[p];
    }
    djikstra_path.push_back(start);
    std::ranges::reverse(djikstra_path);

    return {dist_from_start[end], djikstra_path};
  }

  /// INFO: Strongly connected components (SCC)
  auto scc() -> std::vector<DiGraph> const;
};

/// INFO: A DAG is a Directed Acyclic Graph that can be multi-edges, with
/// edge-cost being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DAG : public DiGraph<NodeType, Cost, CounterType> {
public:
  // A topological sort is a reversed post order
  auto topo_sort() -> std::vector<CounterType> const {
    auto dfs_result = this->template dfs<VisitOrder::post>();
    std::ranges::reverse(dfs_result);
    return dfs_result;
  }
};
template class DiGraph<std::string, float_t, uint16_t>;
template class DAG<std::string, float_t, uint16_t>;
template class DiGraph<uint32_t, uint32_t, uint16_t>;
template class DAG<uint32_t, uint32_t, uint16_t>;
} // namespace lean_graph
/*template class DiGraph<float_t>;*/
