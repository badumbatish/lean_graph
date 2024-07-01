use std::{
    collections::{HashMap, HashSet},
    hash::Hash,
};

use crate::{storage::GraphStorage};

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

    fn remove_vertex(&mut self, vertex: V) {
        self.adjacency_list.remove(&vertex);
        for neighbors in self.adjacency_list.values_mut() {
            neighbors.retain(|(v, _)| v != &vertex);
        }
    }
    fn add_vertex(&mut self, vertex: V) {
        self.adjacency_list
            .entry(vertex.clone())
            .or_insert(Default::default());
    }

    fn add_edge(&mut self, from: V, to: V, edge: E) {
        // Add two vertices to the graph since the representation
        // is adjacency list.
        self.add_vertex(from.clone());
        self.add_vertex(to.clone());
        self.adjacency_list
            .get_mut(&from)
            .unwrap()
            .insert((to, edge));
    }

    // Remove the edge from the adjancency list if it exists
    fn remove_edge(&mut self, from: &V, to: &V, edge: E) {
        if let Some(neighbors) = self.adjacency_list.get_mut(from) {
            neighbors.remove(&(to.clone(), edge));
        }
    }

    fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool {
        self.adjacency_list.contains_key(from)
            && self.adjacency_list[from].contains(&(to.clone(), edge.clone()))
    }

    fn neighbors(&self, vertex: &V) -> Vec<(V, E)> {
        Vec::from_iter(self.adjacency_list[vertex].iter().cloned())
    }

    fn vertices(&self) -> Vec<V> {
        Vec::from_iter(self.adjacency_list.keys().cloned())
    }

    fn edges(&self) -> Vec<E> {
        let mut edges = HashSet::new();
        for neighbors in self.adjacency_list.values() {
            for (_, edge) in neighbors {
                edges.insert(edge.clone());
            }
        }
        Vec::from_iter(edges)
    }

    fn has_vertex(&self, vertex: &V) -> bool {
        self.adjacency_list.contains_key(vertex)
    }
}
