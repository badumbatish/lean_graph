use std::collections::HashMap;

use crate::storage::{Edge, FixedStorage, GraphStorage, Vertex};

pub struct BinaryAdjMatrix {
    matrix: Vec<Vec<usize>>,
    hash: HashMap<Vertex, usize>,
    counter: usize,
    capacity: usize,
}

impl FixedStorage for BinaryAdjMatrix {
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

impl GraphStorage for BinaryAdjMatrix {
    fn edge_size(&self) -> u64 {
        todo!()
    }
    fn vertex_size(&self) -> u64 {
        todo!()
    }

    fn add_vertex(&mut self, vertex: &Vertex) -> bool {
        if self.hash.contains_key(vertex) {
            return true;
        } else if self.counter < self.capacity {
            self.hash.insert(vertex.clone(), self.counter);
            self.counter += 1;
            return true;
        } else {
            return false;
        }
    }

    fn remove_vertex(&mut self, vertex: &Vertex) -> bool {
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

    fn add_edge(&mut self, from: &Vertex, to: &Vertex, _edge: &Edge) -> bool {
        let (i, j) = (self.hash.get(from), self.hash.get(to));
        if i.is_none() || j.is_none() {
            return false;
        } else {
            let i = *i.unwrap();
            let j = *j.unwrap();
            self.matrix[i][j] = 1;
            self.matrix[j][i] = 1;
            return true;
        }
    }

    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, _edge: &Edge) -> bool {
        let (i, j) = (self.hash.get(from), self.hash.get(to));
        if i.is_none() || j.is_none() {
            return false;
        } else {
            let i = *i.unwrap();
            let j = *j.unwrap();
            self.matrix[i][j] = 0;
            self.matrix[j][i] = 0;
            return true;
        }
    }

    fn has_vertex(&self, vertex: &Vertex) -> bool {
        return self.hash.contains_key(vertex);
    }

    fn has_edge(&self, from: &Vertex, to: &Vertex, _edge: &crate::storage::Edge) -> bool {
        return self.hash.contains_key(from) && self.hash.contains_key(to);
    }

    fn neighbors(&self, vertex: &Vertex) -> Option<Vec<(Vertex, Edge)>> {
        todo!()
    }

    fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> Option<bool> {
        let index = self.hash.remove(old_vertex);
        if index.is_none() {
            return Some(false);
        } else {
            self.hash.insert(new_vertex.clone(), index.unwrap());
            return Some(true);
        }
    }

    fn set_edge(
        &mut self,
        _from: &Vertex,
        _to: &Vertex,
        _old_edge: &crate::storage::Edge,
        _new_edge: &crate::storage::Edge,
    ) -> Option<bool> {
        None
    }
}
