use std::collections::{HashMap, HashSet};

use crate::storage::{DynamicStorage, Edge, GraphStorage, Vertex};

pub struct AdjacencyList {
    adj_list: HashMap<Vertex, HashSet<(Vertex, Edge)>>,
}

impl GraphStorage for AdjacencyList {
    fn add_vertex(&mut self, vertex: &Vertex) -> Option<bool> {
        Some(
            self.adj_list
                .insert(vertex.clone(), HashSet::new())
                .is_none(),
        )
    }

    fn remove_vertex(&mut self, vertex: &Vertex) -> bool {
        self.adj_list.remove(vertex).is_some()
    }

    fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool> {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            neighbors.insert(at);
            Some(true)
        } else {
            Some(false)
        }
    }

    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool> {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            Some(neighbors.remove(&at))
        } else {
            Some(false)
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

    fn neighbors(&self, vertex: &Vertex) -> Option<Vec<(Vertex, Edge)>> {
        if let Some(neighbors) = self.adj_list.get(vertex) {
            Some(
                neighbors
                    .iter()
                    .map(|(v, e)| (v.clone(), e.clone()))
                    .collect(),
            )
        } else {
            None
        }
    }

    fn vertex_size(&self) -> u64 {
        self.adj_list.len() as u64
    }

    fn edge_size(&self) -> u64 {
        self.adj_list
            .values()
            .map(|neighbors| neighbors.len())
            .sum::<usize>() as u64
    }

    fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> Option<bool> {
        let mut exists = false;

        for neighbors in self.adj_list.values_mut() {
            let mut to_remove = None;
            let mut to_insert = None;

            for (v, e) in neighbors.iter() {
                if v == old_vertex {
                    exists = true;
                    to_remove = Some((v.clone(), e.clone()));
                    to_insert = Some((new_vertex.clone(), e.clone()));
                    break;
                }
            }

            if let Some(remove_item) = to_remove {
                neighbors.remove(&remove_item);
            }

            if let Some(insert_item) = to_insert {
                neighbors.insert(insert_item);
            }
        }

        if let Some(set) = self.adj_list.remove(old_vertex) {
            self.adj_list.insert(new_vertex.clone(), set);
            exists = true;
        }
        Some(exists)
    }

    fn set_edge(
        &mut self,
        from: &Vertex,
        to: &Vertex,
        old_edge: &crate::storage::Edge,
        new_edge: &crate::storage::Edge,
    ) -> Option<bool> {
        let mut exists = false;

        if self.adj_list.contains_key(from) && self.adj_list.contains_key(to) {
            if let Some(neighbors) = self.adj_list.get_mut(from) {
                let at = (to.clone(), old_edge.clone());
                if neighbors.remove(&at) {
                    let at = (to.clone(), new_edge.clone());
                    neighbors.insert(at);
                    exists = true;
                }
            }
        }

        Some(exists)
    }
}

impl DynamicStorage for AdjacencyList {
    fn new() -> Self {
        AdjacencyList {
            adj_list: HashMap::new(),
        }
    }
}
#[cfg(test)]
mod adj_list_test {
    use crate::{
        storage::DynamicStorage,
        u_graph::{test_graph_initialization, UGraph},
    };

    use super::*;
    #[test]
    fn adj_list_init() {
        let g = &mut UGraph::<AdjacencyList>::new();
        test_graph_initialization(g)
    }
}
