use std::collections::{HashMap, HashSet};

use crate::storage::{DynamicStorage, GraphStorage};

pub struct AdjacencyList<V, E> {
    adj_list: HashMap<V, HashSet<(V, E)>>,
}

impl<V, E> GraphStorage<V, E> for AdjacencyList<V, E>
where
    V: Clone + Eq + std::hash::Hash,
    E: Clone + Eq + std::hash::Hash,
{
    fn add_vertex(&mut self, vertex: &V) -> Option<bool> {
        Some(
            self.adj_list
                .insert(vertex.clone(), HashSet::new())
                .is_none(),
        )
    }

    fn remove_vertex(&mut self, vertex: &V) -> bool {
        self.adj_list.remove(vertex).is_some()
    }

    fn add_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool> {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            neighbors.insert(at);
            Some(true)
        } else {
            Some(false)
        }
    }

    fn remove_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool> {
        if let Some(neighbors) = self.adj_list.get_mut(from) {
            let at = (to.clone(), edge.clone());
            Some(neighbors.remove(&at))
        } else {
            Some(false)
        }
    }

    fn has_vertex(&self, vertex: &V) -> bool {
        self.adj_list.contains_key(vertex)
    }

    fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool {
        if let Some(neighbors) = self.adj_list.get(from) {
            let at = (to.clone(), edge.clone());
            neighbors.contains(&at)
        } else {
            false
        }
    }

    fn neighbors(&self, vertex: &V) -> Option<Vec<(V, E)>> {
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

    fn set_vertex(&mut self, old_vertex: &V, new_vertex: &V) -> Option<bool> {
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

    fn set_edge(&mut self, from: &V, to: &V, old_edge: &E, new_edge: &E) -> Option<bool> {
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

impl<V, E> DynamicStorage for AdjacencyList<V, E> {
    fn new() -> Self {
        AdjacencyList {
            adj_list: HashMap::new(),
        }
    }
}
#[cfg(test)]
mod adj_list_test {
    use crate::adjacency_list::AdjacencyList;
    use crate::storage::{DynamicStorage, GraphStorage};
    #[test]
    fn test_new() {
        let mut adj_list = AdjacencyList::new();
        adj_list.add_vertex(&1);
        adj_list.add_vertex(&2);
        adj_list.add_edge(&1, &2, &3);

        assert_eq!(adj_list.has_vertex(&1), true);
        assert_eq!(adj_list.vertex_size(), 2);
        assert_eq!(adj_list.edge_size(), 1);
    }
}
