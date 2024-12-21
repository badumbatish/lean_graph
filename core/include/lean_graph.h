#pragma once
#include <__expected/unexpected.h>
#include <algorithm>
#include <expected>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>

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
template <class Node, class Cost>
  requires std::is_arithmetic_v<Cost>
class BasicGraph {
public:
  //  INFO: Edges type
  using CounterEdge = std::tuple<CounterType, CounterType, Cost>;
  using CounterHalfEdge = std::tuple<CounterType, Cost>;
  using Edge = std::tuple<Node, Node, Cost>;
  using BlankEdge = std::tuple<Node, Node>;
  using HalfEdge = std::tuple<Node, Cost>;
  //  INFO: Results
  using NodeResult = std::expected<CounterType, node_error>;
  using EdgeResult = std::expected<CounterEdge, node_error>;

private:
  std::map<CounterType, std::set<CounterHalfEdge>> graph;
  Counter<Node> node_counter;

  auto NodeToCounter(Node node) -> NodeResult {
    if (!node_counter.exist(node))
      return std::unexpected<node_error>(node_error::not_exist);
    return node_counter.get_counter(node);
  }

public:
  auto registerNode(Node node) -> CounterType {
    return node_counter.get_counter(node);
  }

  auto registerEdge(Edge edge) -> CounterEdge {
    auto [from, to, cost] = edge;
    CounterEdge result = {node_counter.get_counter(from),
                          node_counter.get_counter(to), cost};

    graph[std::get<0>(result)].insert({std::get<1>(result), cost});
    return result;
  }

  auto existEdge(CounterEdge edge) -> bool {
    auto [from, to, cost] = edge;
    if (!existNode(from))
      return false;
    auto s = graph.find(from);
    if (s == graph.end())
      return false;

    return (*s).contains({to, cost});
  }
  auto existBlankEdge(BlankEdge edge) -> bool {
    auto [from, to] = edge;
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
};

/*template <class T, class C> class UGraph : public BasicGraph<T, C> {};*/
/*template <class T, class C> class DiGraph : public BasicGraph<T, C> {};*/
