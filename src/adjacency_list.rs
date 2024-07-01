use std::collections::{HashMap, HashSet};

use crate::storage::GraphStorage;

pub struct AdjacencyList<V, E> {
    adjacency_list: HashMap<V, HashSet<(V, E)>>,
}

impl<V, E> GraphStorage<V, E> for AdjacencyList<V, E> {
    fn new() -> Self {
        todo!()
    }

    fn add_vertex(&mut self, vertex: V) {
        todo!()
    }

    fn add_edge(&mut self, from: V, to: V, edge: E) {
        todo!()
    }

    fn remove_edge(&mut self, from: &V, to: &V, edge: E) {
        todo!()
    }

    fn has_edge(&self, from: &V, to: &V, edge: E) -> bool {
        todo!()
    }

    fn neighbors(&self, vertex: &V) -> Vec<&V> {
        todo!()
    }
}
