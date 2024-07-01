use std::hash::Hash;
pub enum VertexError {
    VertexExists,
}
pub trait GraphStorage<V, E>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
{
    fn new() -> Self;
    fn add_vertex(&mut self, vertex: V);
    fn add_edge(&mut self, from: V, to: V, edge: E);
    fn remove_edge(&mut self, from: &V, to: &V, edge: E);
    fn has_vertex(&self, vertex: &V) -> bool;
    fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool;
    fn neighbors(&self, vertex: &V) -> Vec<(V, E)>;
    fn vertices(&self) -> Vec<V>;
    fn edges(&self) -> Vec<E>;
}
