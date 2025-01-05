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

/////////////////////////////////////////////////////////////////
/////////////////////////// START DECL SPACE
/////////////////////////////////////////////////////////////////
namespace lean_graph {
// Declaration of the concept “Hashable”, which is satisfied by any type “T”
// such that for values “a” of type “T”, the expression std::hash<T>{}(a)
// compiles and its result is convertible to std::size_t
//
/// TAG: Hashable DECL
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

/// TAG: DefaultHashMap DECL
template <class NodeType, class CounterType>
using DefaultHashMap = std::unordered_map<NodeType, CounterType>;

/// TAG: Counter DECL
template <class Aspect, class CounterType, class H> class Counter;

/// TAG: DiGraph DECL
/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DiGraph;

/// TAG: DAG DECL
/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DAG;

/// TAG: UniGraph DECL
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class UniGraph;

/// TAG: Connectivity DECL
template <class CounterType, class H = DefaultHashMap<CounterType, CounterType>>
class Connectivity;

enum class node_error { not_exist, duplicate, general_error };
enum class edge_error { not_exist, duplicate, general_error };
enum VisitOrder { pre, post };

// TAG: Edge DECL
template <class CounterType, class Cost>
using CounterEdge = std::tuple<CounterType, CounterType, Cost>;
template <class CounterType>
using CounterBlankEdge = std::tuple<CounterType, CounterType>;
template <class CounterType, class Cost>
using CounterHalfEdge = std::tuple<CounterType, Cost>;

template <class CounterType, class Cost>
using Edge = std::tuple<CounterType, CounterType, Cost>;

// TAG: RESULT DECL
template <class CounterType, class Cost>
using NodeResult = std::expected<CounterType, node_error>;
template <class CounterType, class Cost>
using EdgeResult = std::expected<CounterEdge<CounterType, Cost>, node_error>;

} // namespace lean_graph

/////////////////////////////////////////////////////////////////
/////////////////////////// END DECL SPACE
/////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////
/////////////////////////// START DEFN SPACE
/////////////////////////////////////////////////////////////////
namespace lean_graph {

// TAG: Counter DEFN
/// INFO: A counter class
template <class Aspect, class CounterType, class H> class Counter {
  H counter;
  CounterType count;

public:
  explicit Counter(CounterType start_from) : count(start_from) {}
  bool exist(const Aspect &aspect) const {
    return counter.find(aspect) != counter.end();
  }
  bool counter_exceeds(CounterType ct) const { return count > ct; }
  CounterType get_counter(const Aspect &aspect) {
    if (not exist(aspect))
      counter[aspect] = count++;
    return counter[aspect];
  }
};

///
// TAG: DiGraph DEFN
template <class NodeType, class Cost, class CounterType, class H>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DiGraph {
public:
protected:
  std::unordered_map<CounterType, std::set<CounterHalfEdge<CounterType, Cost>>>
      graph;
  Counter<NodeType, CounterType, H> node_counter{0};

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
    if (not existCounterNode(from))
      return {};

    stck.push(tup(from, VisitOrder::pre));
    while (not stck.empty()) {
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
    if (not existNode(from))
      return {};

    q.push(tup(from, VisitOrder::pre));
    while (not q.empty()) {
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
  auto registerNode(const NodeType &node) -> CounterType {
    return node_counter.get_counter(node);
  }

  virtual auto registerEdge(CounterEdge<CounterType, Cost> edge) -> void {
    const auto [from, to, cost] = edge;
    this->graph[from].insert({to, cost});
    return;
  }

  virtual auto modifyEdge(CounterEdge<CounterType, Cost> edge, Cost new_cost)
      -> std::optional<edge_error> {
    if (not this->existEdge(edge)) // if edge doesn't exist
      return edge_error::not_exist;

    const auto &[from, to, old_cost] = edge;
    this->graph[from].erase({to, old_cost});
    this->graph[from].insert({to, new_cost});
    return std::nullopt;
  }

  auto existEdge(CounterEdge<CounterType, Cost> edge) const -> bool {
    auto &[from, to, cost] = edge;
    if (!existCounterNode(from))
      return false;
    auto s = this->graph.find(from);
    if (s == this->graph.end())
      return false;

    return (*s).second.contains({to, cost});
  }
  auto existBlankEdge(CounterBlankEdge<CounterType> edge) const -> bool {
    /// NOTE: To bypass unused var warning in structured bindings
    auto from = std::get<0>(edge), to = std::get<1>(edge);
    /*auto [from, to] = edge;*/
    if (not existCounterNode(from))
      return false;
    auto s = this->graph.find(from);
    if (s == this->graph.end())
      return false;

    auto st = (*s).second;
    return std::find_if(st.begin(), st.end(), [=](const auto &p) {
             return to == std::get<0>(p);
           }) != st.end();
  }
  auto existBlankEdge(CounterEdge<CounterType, Cost> edge) const -> bool {
    auto [from, to, cost] = edge;
    return existBlankEdge({from, to});
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
      if (not visited.contains(node))
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
      if (not visited.contains(node))
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
      -> std::tuple<Cost, std::vector<CounterType>> const {
    if (not existNode(start))
      return {0, {}};
    std::unordered_map<CounterType, Cost> dist_from_start;
    std::unordered_map<CounterType, CounterType> prev;
    dist_from_start[start] = 0;
    std::priority_queue<std::tuple<Cost, CounterType>,
                        std::vector<std::tuple<Cost, CounterType>>,
                        decltype(compare)>
        pq(compare);

    pq.emplace(dist_from_start[start], start);
    while (not pq.empty()) {
      auto [dist_node, node] = pq.top();
      pq.pop();

      for (auto [neighbor, cost] : graph[node]) {
        if (not dist_from_start.contains(neighbor) or
            (dist_from_start[neighbor] > dist_node[node] + cost)) {
          dist_from_start[neighbor] = dist_node[node] + cost;
          prev[neighbor] = node;
          pq.emplace(dist_from_start[neighbor], neighbor);
        }
      }
    }

    if (not prev.contains(end))
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
  auto bellman_ford(auto compare = std::greater<>()) const;

  /// INFO: Strongly connected components (SCC)
  auto scc() -> std::vector<DiGraph> const;
};

/// INFO: A DAG is a Directed Acyclic Graph that can be multi-edges, with
/// edge-cost being different.

// TAG: DAG DEFN
template <class NodeType, class Cost, class CounterType, class H>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DAG : public DiGraph<NodeType, Cost, CounterType, H> {
public:
  // A topological sort is a reversed post order
  auto topo_sort() -> std::vector<CounterType> const {
    auto dfs_result = this->template dfs<VisitOrder::post>();
    std::ranges::reverse(dfs_result);
    return dfs_result;
  }
};

// TAG: UniGraph DEFN
template <class NodeType, class Cost, class CounterType, class H>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class UniGraph : DiGraph<NodeType, Cost, CounterType, H> {

public:
  auto registerEdge(CounterEdge<CounterType, Cost> edge) -> void override {
    const auto [from, to, cost] = edge;
    this->graph[from].insert({to, cost});
    this->graph[to].insert({from, cost});
    return;
  }

  auto modifyEdge(CounterEdge<CounterType, Cost> edge, Cost new_cost)
      -> std::optional<edge_error> override {

    if (not this->existEdge(edge)) // if edge doesn't exist
      return edge_error::not_exist;

    const auto &[from, to, old_cost] = edge;
    this->graph[from].erase({to, old_cost});
    this->graph[from].insert({to, new_cost});
    this->graph[to].erase({from, old_cost});
    this->graph[to].insert({from, new_cost});
    return std::nullopt;
  }

  /*Connectivity<CounterType> getConnectivityInfo() {*/
  /*  Connectivity<CounterType> c;*/
  /*  for (auto &[node, neighbors] : this->graph) {*/
  /*    for (auto [neighbor, cost] : neighbors)*/
  /*      c.unite(node, neighbor);*/
  /*  }*/
  /*  return c;*/
  /*}*/
};
// A topological sort is a reversed post order

/// TAG: CONNECTIVITY DEFN
/// INFO: Connectivity is just glorified union find, this is from DPV book
template <class CounterType, class H> class Connectivity {
  H uf;
  std::unordered_map<CounterType, uint32_t> rank;

  /// INFO: find with path compression
  CounterType find(CounterType a) {
    if (uf.find(a) == uf.end()) // lazy rank computation
      return a;

    if (a != uf[a])
      uf[a] = find(uf[a]);
    return uf[a];
  }

  /// INFO: Unite (Union) a with b
  void unite(CounterType a, CounterType b) {
    auto ra = this->find(a);
    auto rb = this->find(b);

    if (ra == rb)
      return;
    if (rank[ra] > rank[rb])
      uf[rb] = ra;
    else {
      uf[ra] = uf[rb];
      if (rank[ra] == rank[rb])
        rank[rb] = rank[rb] + 1;
    }
  }

public:
  bool is_connected(CounterType a, CounterType b) {
    return this->find(a) == this->find(b);
  }

  template <class NodeType, class Cost, class C>
    requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
  friend class UniGraph;
};
/////////////////////////////////////////////////////////////////
/////////////////////////// END DEFN SPACE
/////////////////////////////////////////////////////////////////
} // namespace lean_graph
