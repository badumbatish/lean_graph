#include "lean_graph.h"
#include <iostream>

int main() {

  BasicGraph<int, int> g;
  auto a = g.registerNode(1);
  auto b = g.registerNode(2);
  auto e = g.registerEdge({1, 2, 0});

  if (g.existBlankEdge({a, b})) {
    std::cout << "True" << std::endl;
  } else {
    std::cout << "False" << std::endl;
  }
  return 0;
}
