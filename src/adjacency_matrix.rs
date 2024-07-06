use std::collections::HashMap;

use crate::storage::{FixedStorage, GraphStorage, Vertex};

pub struct AdjacencyMatrix {
    matrix: Vec<Vec<usize>>,
    hash: HashMap<Vertex, usize>,
    counter: usize,
    capacity: usize,
}

impl FixedStorage for AdjacencyMatrix {
    fn new(size: u32) -> Self {
        let ma = vec![vec![0; size as usize]; size as usize];

        AdjacencyMatrix {
            matrix: ma,
            hash: HashMap::new(),
            counter: 0,
            capacity: size as usize,
        }
    }
}

impl GraphStorage for AdjacencyMatrix {
    fn edge_size(&self) -> u64 {
        todo!()
    }
    fn vertex_size(&self) -> u64 {
        todo!()
    }

    fn add_vertex(&mut self, vertex: &Vertex) -> bool {
        todo!()
    }

    fn remove_vertex(&mut self, vertex: &Vertex) -> bool {
        todo!()
    }

    fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &crate::storage::Edge) -> bool {
        todo!()
    }

    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &crate::storage::Edge) -> bool {
        todo!()
    }

    fn has_vertex(&self, vertex: &Vertex) -> bool {
        todo!()
    }

    fn has_edge(&self, from: &Vertex, to: &Vertex, edge: &crate::storage::Edge) -> bool {
        todo!()
    }

    fn neighbors(&self, vertex: &Vertex) -> Vec<(Vertex, crate::storage::Edge)> {
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
