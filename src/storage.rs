pub trait GraphStorage<V, E> {
    fn new() -> Self;
    fn add_vertex(&mut self, vertex: V);
    fn add_edge(&mut self, from: V, to: V, edge: E);
    fn remove_edge(&mut self, from: &V, to: &V, edge: E);
    fn has_edge(&self, from: &V, to: &V, edge: E) -> bool;
    fn neighbors(&self, vertex: &V) -> Vec<&V>;
}
