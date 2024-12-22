#include "lean_graph.h"
#include <cstdint>
#include <iostream>
template <class C, class T> void make_graph() {

  DiGraph<C, T> graph;
  auto a = graph.registerNode(0);
  auto b = graph.registerNode(1);
  auto c = graph.registerNode(2);
  auto d = graph.registerNode(3);
  auto e = graph.registerNode(4);
  auto f = graph.registerNode(5);
  auto g = graph.registerNode(6);

  auto ab = graph.registerEdge({a, b, 0});
  auto ac = graph.registerEdge({a, c, 0});
  auto bd = graph.registerEdge({b, d, 0});
  auto be = graph.registerEdge({b, e, 0});
  auto cf = graph.registerEdge({c, f, 0});
  auto cg = graph.registerEdge({c, g, 0});

  if (graph.existBlankEdge(ab)) {
    std::cout << "True" << std::endl;
  } else {
    std::cout << "False" << std::endl;
  }

  auto print_node = [](CounterType node) {
    std::cout << node << std::endl;
    return true;
  };
  std::cout << "Printing dfs\n";
  graph.dfs(a, print_node, nullptr);
  std::cout << "Printing bfs\n";
  graph.bfs(a, print_node, nullptr);
}
int main() {

  make_graph<uint32_t, uint32_t>();
  make_graph<uint16_t, uint16_t>();

  return 0;
}
