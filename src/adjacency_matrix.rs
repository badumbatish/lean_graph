use std::collections::HashMap;

use crate::storage::{FixedStorage, GraphStorage};

pub struct BinaryAdjMatrix<V> {
    matrix: Vec<Vec<usize>>,
    hash: HashMap<V, usize>,
    counter: usize,
    capacity: usize,
}

impl<V> FixedStorage for BinaryAdjMatrix<V> {
    fn new(size: u32) -> Self {
        let ma = vec![vec![0; size as usize]; size as usize];

        BinaryAdjMatrix {
            matrix: ma,
            hash: HashMap::new(),
            counter: 0,
            capacity: size as usize,
        }
    }
}

impl<V, E> GraphStorage<V, E> for BinaryAdjMatrix<V>
where
    V: Clone + Eq + std::hash::Hash,
{
    fn edge_size(&self) -> u64 {
        todo!()
    }
    fn vertex_size(&self) -> u64 {
        todo!()
    }

    fn add_vertex(&mut self, vertex: &V) -> Option<bool> {
        // If it contains the vertex, return false
        if self.hash.contains_key(vertex) {
            return Some(false);
        } else if self.counter < self.capacity {
            // If we still have space, insert and returns true
            self.hash.insert(vertex.clone(), self.counter);
            self.counter += 1;
            return Some(true);
        } else {
            return None;
        }
    }

    fn remove_vertex(&mut self, vertex: &V) -> bool {
        if self.hash.contains_key(vertex) {
            let index = self.hash.remove(vertex).unwrap();
            for i in 0..self.capacity {
                self.matrix[index][i] = 0;
                self.matrix[i][index] = 0;
            }
            return true;
        } else {
            return false;
        }
    }

    fn add_edge(&mut self, from: &V, to: &V, _edge: &E) -> Option<bool> {
        let (i, j) = (self.hash.get(from), self.hash.get(to));
        if i.is_none() || j.is_none() {
            return Some(false);
        } else {
            let i = *i.unwrap();
            let j = *j.unwrap();
            self.matrix[i][j] = 1;
            self.matrix[j][i] = 1;
            return Some(true);
        }
    }

    fn remove_edge(&mut self, from: &V, to: &V, _edge: &E) -> Option<bool> {
        let (i, j) = (self.hash.get(from), self.hash.get(to));
        if i.is_none() || j.is_none() {
            return Some(false);
        } else {
            let i = *i.unwrap();
            let j = *j.unwrap();
            self.matrix[i][j] = 0;
            self.matrix[j][i] = 0;
            return Some(true);
        }
    }

    fn has_vertex(&self, vertex: &V) -> bool {
        return self.hash.contains_key(vertex);
    }

    fn has_edge(&self, from: &V, to: &V, _edge: &E) -> bool {
        return self.hash.contains_key(from) && self.hash.contains_key(to);
    }

    fn neighbors(&self, vertex: &V) -> Option<Vec<(V, E)>> {
        todo!()
    }

    fn set_vertex(&mut self, old_vertex: &V, new_vertex: &V) -> Option<bool> {
        let index = self.hash.remove(old_vertex);
        if index.is_none() {
            return Some(false);
        } else {
            self.hash.insert(new_vertex.clone(), index.unwrap());
            return Some(true);
        }
    }

    fn set_edge(&mut self, _from: &V, _to: &V, _old_edge: &E, _new_edge: &E) -> Option<bool> {
        None
    }
}
