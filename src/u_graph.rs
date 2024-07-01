use crate::storage::GraphStorage;
use std::hash::Hash;
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

    pub fn add_edge(&mut self, from: V, to: V, edge: E) {
        self.storage.add_edge(from, to, edge);
    }

    pub fn edges(&self) -> Vec<E> {
        self.storage.edges()
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
    }

    fn test_vertices_template<S: GraphStorage<i32, i32>>() {
        let mut graph = UGraph::<i32, i32, S>::new();
        assert!(graph.edges().is_empty());

        graph.add_vertex(1, 2, 3);
        assert!(graph.edges().contains(&3));

        graph.add_edge(2, 3, 4);
        assert!(graph.edges().contains(&4));

        assert_eq!(graph.edges().len(), 2);
    }

    #[test]
    fn test_edges() {
        test_edges_template::<crate::adjacency_list::AdjacencyList<i32, i32>>();
    }
}
