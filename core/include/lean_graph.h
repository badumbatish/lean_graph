#pragma once
#include <__expected/unexpected.h>
#include <algorithm>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
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
// TAG: Hashable DECL
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// TAG: DefaultHashMap DECL
template <class NodeType, class CounterType>
using DefaultHashMap = std::unordered_map<NodeType, CounterType>;

// TAG: Counter DECL
template <class Aspect, class CounterType, class H> class Counter;

// TAG: DiGraph DECL
/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DiGraph;

// TAG: DAG DECL
/// INFO: A BasicGraph is just a DiGraph that can be multi-edges, with edge-cost
/// being different.
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DAG;

// TAG: UniGraph DECL
template <class NodeType, class Cost = float_t,
          class CounterType = std::uint16_t,
          class H = DefaultHashMap<NodeType, CounterType>>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class UniGraph;

// TAG: Connectivity DECL
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
  CounterType get_counter() const { return count; }
};

template <class CounterType, class Cost> class EdgeIte {};
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
  CounterType num_node, num_edge;
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
    if (not existCounterNode(from))
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
    if (!node_counter.exist(node))
      num_node++;
    return node_counter.get_counter(node);
  }

  virtual auto registerEdge(CounterEdge<CounterType, Cost> edge) -> void {
    const auto [from, to, cost] = edge;
    if (!existEdge(edge))
      num_edge++;
    this->graph[from].insert({to, cost});
    return;
  }

  virtual auto modifyEdge(CounterEdge<CounterType, Cost> edge, Cost new_cost)
      -> std::optional<edge_error> {
    if (not this->existEdge(edge)) // if edge doesn't exist
      return edge_error::not_exist;

    const auto &[from, to, old_cost] = edge;
    const CounterEdge<CounterType, Cost> new_edge = {from, to, new_cost};
    this->graph[from].erase({to, old_cost});
    num_edge--;
    if (!this->existEdge(new_edge))
      num_edge++;
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

  virtual auto edges() const -> std::vector<CounterEdge<CounterType, Cost>> {
    decltype(edges()) result;
    for (auto &[node, neighbors_info] : this->graph) {
      for (auto &[to_neighbor, cost] : neighbors_info) {
        result.push_back({node, to_neighbor, cost});
      }
    }
    return result;
  }
  /// INFO: Performs full dfs of all nodes connected to a node
  /// with either pre or post order from a single node
  template <VisitOrder v>
  [[nodiscard("\nDON'T DISCARD THE RESULT OF dfs() - DEPTH FIRST "
              "SEARCH ON THE WHOLE GRAPH.\n")]]
  auto dfs() const -> std::vector<CounterType> {
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
  template <VisitOrder v>
  [[nodiscard("\nDON'T DISCARD THE RESULT OF bfs() - BREADTH FIRST "
              "SEARCH ON THE WHOLE GRAPH.\n")]]
  auto bfs() const -> std::vector<CounterType> {
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
  [[nodiscard("\nDON'T DISCARD THE RESULT OF explore_dfs() - DEPTH FIRST "
              "SEARCH OF A SINGULAR NODE.\n")]]
  auto explore_dfs(CounterType from) const -> std::vector<CounterType> {
    return explore_dfs_protected<v>(from, std::nullopt);
  }

  /// INFO: Performs exploration of all nodes connected to a node in bfs fashion
  /// with either pre or post order as template from a single node
  template <VisitOrder v>
  [[nodiscard("\nDON'T DISCARD THE RESULT OF explore_bfs() - BREADTH FIRST "
              "SEARCH OF A SINGULAR NODE.\n")]]
  auto explore_bfs(CounterType from) const -> std::vector<CounterType> {
    return explore_bfs_protected<v>(from, std::nullopt);
  }

  /// INFO: Johnson algorithm
  [[nodiscard("\nDON'T DISCARD THE RESULT OF cycles(), WHICH RETURNS A VECTOR "
              "OF ELEMENTARY CYCLES\n")]]
  virtual auto /* DiGraph */ cycles() const
      -> std::vector<std::vector<CounterType>> {
    return {};
  }
  /// INFO: Single source, single path dijkstra algorithm
  /// User discretion required, user might input negative cost.
  ///
  /// Successful djikstra will contain at least a vector of two nodes.
  /// If you're not a nerd, please be careful
  [[nodiscard(
      "\nDon't discard the result of djikstra's singular shorest path.\n")]]
  virtual auto /* DiGraph */ singular_shortest_path(CounterType start,
                                                    CounterType end) const
      -> std::pair<std::unordered_map<CounterType, Cost>,
                   std::unordered_map<CounterType, CounterType>> {
    if (not existCounterNode(start))
      return {{}, {}};
    std::unordered_map<CounterType, Cost> dist_from_start;
    std::unordered_map<CounterType, CounterType> prev;
    dist_from_start[start] = 0;
    std::priority_queue<std::tuple<Cost, CounterType>,
                        std::vector<std::tuple<Cost, CounterType>>,
                        decltype(std::greater<>())>
        pq(std::greater<>{});

    pq.emplace(dist_from_start[start], start);
    while (not pq.empty()) {
      auto [dist_node, node] = pq.top();
      pq.pop();

      for (auto [neighbor, cost] : (*graph.find(node)).second) {
        if (not dist_from_start.contains(neighbor) or
            (dist_from_start[neighbor] > dist_from_start[node] + cost)) {
          dist_from_start[neighbor] = dist_from_start[node] + cost;
          prev[neighbor] = node;
          pq.emplace(dist_from_start[neighbor], neighbor);
        }
      }
    }

    if (not prev.contains(end))
      return {{}, {}};

    return {dist_from_start, prev};
  }

  /// INFO: Single source, multi paths bellman ford algorithm
  /// User discretion required, user might input negative cost cycles.
  ///
  /// Successful bellman ford will contain pair of non empty maps.
  /// If you're not a nerd, please be careful
  [[nodiscard("\nDon't discard the result of bellman_ford\n")]]
  auto bellman_ford(CounterType start) const
      -> std::pair<std::unordered_map<CounterType, Cost>,
                   std::unordered_map<CounterType, CounterType>> const {
    std::unordered_map<CounterType, Cost> cost_map;
    std::unordered_map<CounterType, CounterType> prev;

    auto edges = this->edges();

    auto num_vertex_minus_1 = this->num_node - 1;
    cost_map[start] = 0;
    while (num_vertex_minus_1) {
      for (auto &[from, to, cost] : edges) {
        if (not cost_map.contains(from) && not cost_map.contains(to))
          continue;
        if (not cost_map.contains(from)) // if we don't do this, big fat ass
          continue;                      // trouble of over-flowing

        auto &a = cost_map[from];
        auto b = cost_map.contains(to) ? cost_map[to]
                                       : std::numeric_limits<Cost>::max();

        if (a + cost < b) {
          a = a + cost;
          prev[to] = from;
        }
      }

      num_vertex_minus_1--;
    }
    for (auto &[from, to, cost] : edges) {
      if (not cost_map.contains(from) && not cost_map.contains(to))
        continue;
      if (not cost_map.contains(from))
        continue;

      auto &a = cost_map[from];
      auto b = cost_map.contains(to) ? cost_map[to]
                                     : std::numeric_limits<Cost>::max();

      // INFO: negative cycle detected
      if (a + cost < b)
        return {};
    }

    return {cost_map, prev};
  }

  /// INFO: Strongly connected components (SCC)
  auto scc() -> std::vector<DiGraph> const;
  template <class CT, class Cst> friend class EdgeIte;
};

/// INFO: A DAG is a Directed Acyclic Graph that can be multi-edges, with
/// edge-cost being different.

// TAG: DAG DEFN
template <class NodeType, class Cost, class CounterType, class H>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class DAG : public DiGraph<NodeType, Cost, CounterType, H> {
public:
  // A topological sort is a reversed post order
  auto /* DAG */ topo_sort() const -> std::vector<CounterType> {
    auto dfs_result = this->template dfs<VisitOrder::post>();
    std::ranges::reverse(dfs_result);
    return dfs_result;
  }

  virtual auto /* DAG */ singular_shortest_path(CounterType start,
                                                CounterType end) const
      -> std::pair<std::unordered_map<CounterType, Cost>,
                   std::unordered_map<CounterType, CounterType>> override {

    if (not this->existCounterNode(start))
      return {{}, {}};
    std::unordered_map<CounterType, Cost> dist_from_start;
    std::unordered_map<CounterType, CounterType> prev;
    dist_from_start[start] = 0;

    auto linearized_graph_nodes = this->topo_sort();

    for (auto node : linearized_graph_nodes) {
      for (auto [neighbor, cost] : (*this->graph.find(node)).second) {

        if (not dist_from_start.contains(neighbor) or
            (dist_from_start[neighbor] > dist_from_start[node] + cost)) {
          dist_from_start[neighbor] = dist_from_start[node] + cost;
          prev[neighbor] = node;
        }
      }
    }

    // if in the end we don't have the end node, sth seriously wrong with you or
    // me hahahah
    if (not prev.contains(end))
      return {{}, {}};

    return {dist_from_start, prev};
  }
};
// TAG: UniGraph DEFN
template <class NodeType, class Cost, class CounterType, class H>
  requires Hashable<NodeType> and std::is_arithmetic_v<Cost>
class UniGraph : DiGraph<NodeType, Cost, CounterType, H> {

public:
  auto /* UniGraph */ registerEdge(CounterEdge<CounterType, Cost> edge)
      -> void override {
    if (!this->existEdge(edge))
      this->num_edge++;
    const auto [from, to, cost] = edge;
    this->graph[from].insert({to, cost});
    this->graph[to].insert({from, cost});
    return;
  }

  auto /* UniGraph */ modifyEdge(CounterEdge<CounterType, Cost> edge,
                                 Cost new_cost)
      -> std::optional<edge_error> override {
    const auto &[from, to, old_cost] = edge;

    decltype(edge) forward_edge = {from, to, old_cost};
    decltype(edge) backward_edge = {to, from, old_cost};

    using dg = DiGraph<NodeType, Cost, CounterType, H>;

    if (dg::modifyEdge(forward_edge, new_cost) == edge_error::not_exist)
      return edge_error::not_exist;

    return dg::modifyEdge(backward_edge, new_cost);
  }

  auto /* UniGraph */ edges() const
      -> std::vector<CounterEdge<CounterType, Cost>> override {
    decltype(edges()) result;
    std::unordered_set<CounterType> processed;
    for (auto &[node, neighbors_info] : this->graph) {
      for (auto &[to_neighbor, cost] : neighbors_info) {
        if (node != to_neighbor && processed.contains(node))
          continue;
        result.push_back({node, to_neighbor, cost});
      }
      processed.insert(node);
    }
    return result;
  }
  auto /* UniGraph */ cycles() const
      -> std::vector<std::vector<CounterType>> override {
    return {};
  }
  auto /* UniGraph */ mst_kruskal()
      -> std::vector<CounterEdge<CounterType, Cost>> {
    Connectivity<CounterType, H> conn;

    auto edges = this->edges();
    std::priority_queue<CounterEdge<CounterType, Cost>,
                        std::vector<CounterEdge<CounterType, Cost>>,
                        decltype([](auto a, auto b) {
                          return std::get<2>(a) > std::get<2>(b);
                        })>
        pq{};
    pq.push_range(edges);

    decltype(mst_kruskal()) mst;
    while (!pq.empty()) {
      auto [from, to, cost] = pq.top();
      pq.pop();
      if (not conn.is_connected(from, to)) {
        conn.unite(from, to);
        mst.push_back({from, to, cost});
      }
    }

    return mst;
  }
};
// A topological sort is a reversed post order

// TAG: CONNECTIVITY DEFN
/// INFO: Connectivity is just glorified union find, this is from DPV book
template <class CounterType, class H> class Connectivity {
  H uf;
  std::unordered_map<CounterType, uint32_t> rank;

  /// INFO: find with path compression
  CounterType /* Connectivity */ find(CounterType a) {
    if (uf.find(a) == uf.end()) // lazy rank computation
      return a;

    if (a != uf[a])
      uf[a] = find(uf[a]);
    return uf[a];
  }

public:
  bool /* Connectivity */ is_connected(CounterType a, CounterType b) {
    return this->find(a) == this->find(b);
  }

  /// INFO: Unite (Union) a with b
  //
  void /* Connectivity */ unite(CounterType a, CounterType b) {
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
};
/////////////////////////////////////////////////////////////////
/////////////////////////// END DEFN SPACE
/////////////////////////////////////////////////////////////////
} // namespace lean_graph
