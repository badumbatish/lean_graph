use std::{
    collections::{HashMap, HashSet},
    hash::Hash,
};

use crate::storage::GraphStorage;
use crate::storage::VertexError;

pub struct AdjacencyList<V, E> {
    adjacency_list: HashMap<V, HashSet<(V, E)>>,
}

impl<V, E> GraphStorage<V, E> for AdjacencyList<V, E>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
{
    fn new() -> Self {
        AdjacencyList {
            adjacency_list: Default::default(),
        }
    }

    fn add_vertex(&mut self, vertex: V) {
        self.adjacency_list
            .entry(vertex.clone())
            .or_insert(Default::default());
    }

    fn add_edge(&mut self, from: V, to: V, edge: E) {
        self.add_vertex(from.clone());
        self.adjacency_list
            .get_mut(&from)
            .unwrap()
            .insert((to, edge));
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

    fn vertices(&self) -> Vec<&V> {
        todo!()
    }

    fn edges(&self) -> Vec<&E> {
        todo!()
    }
}
