use crate::storage::{DynamicStorage, Edge, Vertex};
use crate::storage::{FixedStorage, GraphStorage};
use delegate::delegate;
pub struct UGraph<S>
where
    S: GraphStorage,
{
    storage: S,
}
impl<S> GraphStorage for UGraph<S>
where
    S: GraphStorage,
{
    fn edge_size(&self) -> u64 {
        todo!()
    }

    fn vertex_size(&self) -> u64 {
        todo!()
    }

    delegate! {
        to self.storage {
            fn add_vertex(&mut self, vertex: &Vertex) -> Option<bool>;
            fn remove_vertex(&mut self, vertex: &Vertex) -> bool;
            fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool>;
            fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool>;
            fn has_vertex(&self, vertex: &Vertex) -> bool;
            fn has_edge(&self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool;
            fn neighbors(&self, vertex: &Vertex) -> Option<Vec<(Vertex, Edge)>>;
            fn set_edge(
                &mut self,
                from: &Vertex,
                to: &Vertex,
                old_edge: &Edge,
                new_edge: &Edge
            ) -> Option<bool>;
            fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> Option<bool>;
        }
    }
}

impl<S> FixedStorage for UGraph<S>
where
    S: GraphStorage + FixedStorage,
{
    fn new(size: u32) -> Self {
        UGraph {
            storage: S::new(size),
        }
    }
}

impl<S> DynamicStorage for UGraph<S>
where
    S: GraphStorage + DynamicStorage,
{
    fn new() -> Self {
        UGraph { storage: S::new() }
    }
}
#[cfg(test)]
mod u_graph {
    use crate::adjacency_matrix::BinaryAdjMatrix;

    use super::*;
    #[test]
    fn test() {}
}

#[test]
fn a() {}
