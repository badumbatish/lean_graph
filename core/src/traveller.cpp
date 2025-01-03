
#include <unordered_map>
template <class NodeType, class Cost, class CounterType,
          class H = std::unordered_map<int, int>>
class DiGraph {};

template <class Aspect, class CounterType, class H> class Counter {
  H counter;
  CounterType count;

public:
  explicit Counter(CounterType start_from) : count(start_from) {}
  bool exist(Aspect aspect) const {
    return counter.find(aspect) != counter.end();
  }
  bool counter_exceeds(CounterType aspect) const { return count > aspect; }
  CounterType get_counter(Aspect aspect) {
    if (not exist(aspect))
      counter[aspect] = count++;
    return counter[aspect];
  }
};
int main() { return 0; }
