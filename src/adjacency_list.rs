use std::collections::{HashMap, HashSet};

use crate::storage::{Edge, GraphStorage, Vertex};

pub struct AdjacencyList {
    adj_list: HashMap<Vertex, HashSet<(Vertex, Edge)>>,
}

impl GraphStorage for AdjacencyList {
    fn add_vertex(&mut self, vertex: &Vertex) -> bool {
        self.adj_list
            .insert(vertex.clone(), HashSet::new())
            .is_none()
    }

    fn remove_vertex(&mut self, vertex: &Vertex) -> bool {
        self.adj_list.remove(vertex).is_some()
    }

    fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            neighbors.insert(at);
            true
        } else {
            false
        }
    }

    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            neighbors.remove(&at)
        } else {
            false
        }
    }

    fn has_vertex(&self, vertex: &Vertex) -> bool {
        self.adj_list.contains_key(vertex)
    }

    fn has_edge(&self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool {
        if let Some(neighbors) = self.adj_list.get(from) {
            let at = (to.clone(), edge.clone());
            neighbors.contains(&at)
        } else {
            false
        }
    }

    fn neighbors(&self, vertex: &Vertex) -> Vec<(Vertex, Edge)> {
        if let Some(neighbors) = self.adj_list.get(vertex) {
            neighbors
                .iter()
                .map(|(v, e)| (v.clone(), e.clone()))
                .collect()
        } else {
            Vec::new()
        }
    }

    fn vertex_size(&self) -> u64 {
        todo!()
    }

    fn edge_size(&self) -> u64 {
        todo!()
    }

    fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> bool {
        todo!()
    }

    fn set_edge(
        &mut self,
        from: &Vertex,
        to: &Vertex,
        old_edge: &crate::storage::Edge,
        new_edge: &crate::storage::Edge,
    ) -> bool {
        todo!()
    }
}
