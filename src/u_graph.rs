use crate::storage::GraphStorage;
use std::{
    hash::Hash,
};
pub struct UGraph<V, E, S>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
    S: GraphStorage<V, E>,
{
    pub storage: S,
    _phantom: std::marker::PhantomData<(V, E)>,
}

impl<V, E, S> UGraph<V, E, S>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
    S: GraphStorage<V, E>,
{
    pub fn new() -> Self {
        UGraph {
            storage: S::new(),
            _phantom: std::marker::PhantomData,
        }
    }
    pub fn neighbors(&self, vertex: &V) -> Vec<(V, E)> {
        self.storage.neighbors(vertex)
    }

    pub fn has_vertex(&self, vertex: &V) -> bool {
        self.storage.vertices().contains(vertex)
    }

    pub fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool {
        self.storage.has_edge(from, to, edge)
    }

    pub fn remove_edge(&mut self, from: &V, to: &V, edge: E) {
        self.storage.remove_edge(from, to, edge);
    }

    pub fn remove_vertex(&mut self, vertex: V) {
        self.storage.remove_vertex(vertex);
    }

    pub fn add_edge(&mut self, from: V, to: V, edge: E) {
        self.storage.add_edge(from, to, edge);
    }

    pub fn edges(&self) -> Vec<E> {
        self.storage.edges()
    }

    pub fn vertices(&self) -> Vec<V> {
        self.storage.vertices()
    }
    pub fn add_vertex(&mut self, vertex: V) {
        self.storage.add_vertex(vertex);
    }
}
#[cfg(test)]

mod u_graph_test_i32 {

    use super::*;

    fn test_edges_template<S: GraphStorage<i32, i32>>() {
        let mut graph = UGraph::<i32, i32, S>::new();
        assert!(graph.edges().is_empty());

        graph.add_edge(1, 2, 3);
        assert!(graph.edges().contains(&3));

        graph.add_edge(2, 3, 4);
        assert!(graph.edges().contains(&4));

        assert_eq!(graph.edges().len(), 2);

        graph.add_edge(1, 2, 5);
        assert!(graph.edges().contains(&5));
        assert_eq!(graph.edges().len(), 3);

        graph.add_edge(1, 2, 3);
        assert_eq!(graph.edges().len(), 3);
    }

    fn test_vertices_template<S: GraphStorage<i32, i32>>() {
        let mut graph = UGraph::<i32, i32, S>::new();
        assert!(graph.vertices().is_empty());

        graph.add_vertex(1);
        assert!(graph.vertices().contains(&1));
    }

    #[test]
    fn test_adj_list() {
        test_edges_template::<crate::adjacency_list::AdjacencyList<i32, i32>>();
        test_vertices_template::<crate::adjacency_list::AdjacencyList<i32, i32>>();
    }
}
