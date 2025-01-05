#include "lean_graph.h"
#include <cstdint>
#include <iostream>
#include <unordered_map>

namespace LG = lean_graph;
namespace lean_graph {
template class DiGraph<std::string, float_t, uint16_t>;
template class DAG<std::string, float_t, uint16_t>;
template class DiGraph<uint32_t, uint32_t, uint16_t>;
template class DAG<uint32_t, uint32_t, uint16_t>;
template class UniGraph<uint32_t, uint32_t, uint16_t>;

} // namespace lean_graph
template <class N, class C, class T> void make_graph() {

  LG::DiGraph<N, C, T, std::unordered_map<int, int>> graph;
  auto a = graph.registerNode(0);
  auto b = graph.registerNode(1);
  auto c = graph.registerNode(2);
  auto d = graph.registerNode(3);
  auto e = graph.registerNode(4);
  auto f = graph.registerNode(5);
  auto g = graph.registerNode(6);

  graph.registerEdge({a, b, 0});
  graph.registerEdge({a, c, 0});
  graph.registerEdge({b, d, 0});
  graph.registerEdge({b, e, 0});
  graph.registerEdge({c, f, 0});
  graph.registerEdge({c, g, 0});

  if (graph.existBlankEdge({a, b})) {
    std::cout << "True" << std::endl;
  } else {
    std::cout << "False" << std::endl;
  }
  auto print_node = [](auto nodes) {
    for (auto node : nodes) {
      std::cout << node << " ";
    }
    std::cout << std::endl;
  };

  auto dfs_pre = graph.template explore_dfs<LG::VisitOrder::pre>(a);
  auto dfs_post = graph.template explore_dfs<LG::VisitOrder::post>(a);
  print_node(dfs_pre);

  auto full_dfs = graph.template dfs<LG::VisitOrder::pre>();
  print_node(full_dfs);
  /*auto if (auto a = std::get<std::vector<T>>(dfs_pre)) { print_node(a); }*/
  /*print_node(dfs_post);*/
}

int main() {

  make_graph<uint32_t, uint32_t, uint32_t>();
  /*make_graph<uint32_t, uint16_t, uint16_t>();*/

  return 0;
}
