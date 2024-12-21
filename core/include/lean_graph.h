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
  bool exist(Aspect aspect) { return counter.find(aspect) != counter.end(); }
  CounterType get_counter(Aspect aspect) {
    if (!exist(aspect))
      counter[aspect] = count++;
    return counter[aspect];
  }

  bool counter_exceeds(CounterType ceiling) { return count > ceiling; }
};

/// INFO: A BasicGraph is just a DiGraph
template <class CounterType, class Cost>
  requires std::is_arithmetic_v<Cost>
class BasicGraph {
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

private:
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
    return node_counter.get_counter(node);
  }

  auto registerEdge(CounterEdge edge) -> CounterEdge {
    const auto &[from, to, cost] = edge;
    graph[from].insert({to, cost});
    return edge;
  }

  auto existEdge(CounterEdge edge) -> bool {
    auto &[from, to, cost] = edge;
    if (!existNode(from))
      return false;
    auto s = graph.find(from);
    if (s == graph.end())
      return false;

    return (*s).contains({to, cost});
  }
  auto existBlankEdge(CounterBlankEdge edge) -> bool {
    auto &[from, to] = edge;
    if (!existNode(from))
      return false;
    auto s = graph.find(from);
    if (s == graph.end())
      return false;

    auto st = (*s).second;
    return std::find_if(st.begin(), st.end(), [to](const auto &p) {
             return to == std::get<0>(p);
           }) != st.end();
  }
  auto existNode(CounterType node) -> bool {
    return node_counter.counter_exceeds(node);
  }
  /// INFO: Performs dfs with 2 callables of pre, and post order
  auto dfs(CounterType from, std::function<bool(CounterType)> pre,
           std::function<bool(CounterType)> post) -> std::optional<node_error> {
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
        for (auto &[neighbor, cost] : graph[current_node]) {
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
           std::function<bool(CounterType)> post) -> std::optional<node_error> {
    using tup = std::tuple<CounterType, VisitOrder>;
    std::queue<tup> q;
    std::unordered_set<CounterType> visited;
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
        for (auto &[neighbor, cost] : graph[current_node]) {
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
};

/*template <class T, class C> class UGraph : public BasicGraph<T, C> {};*/
/*template <class T, class C> class DiGraph : public BasicGraph<T, C> {};*/
