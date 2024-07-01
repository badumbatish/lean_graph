use crate::storage::GraphStorage;

use std::hash::Hash;
pub struct DiGraph<V, E, S>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
    S: GraphStorage<V, E>,
{
    storage: S,
    _phantom: std::marker::PhantomData<(V, E)>,
}

impl<V, E, S> DiGraph<V, E, S>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
    S: GraphStorage<V, E>,
{
    pub fn new() -> Self {
        DiGraph {
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

    pub fn vertices(&self) -> Vec<V> {
        self.storage.vertices()
    }
    pub fn add_vertex(&mut self, vertex: V) {
        self.storage.add_vertex(vertex);
    }
}
