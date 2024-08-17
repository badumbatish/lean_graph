use std::marker::PhantomData;

use crate::storage::DynamicStorage;
use crate::storage::{FixedStorage, GraphStorage};
use delegate::delegate;
pub struct UGraph<S, V, E>
where
    S: GraphStorage<V, E>,
{
    storage: S,
    _phantom_e: PhantomData<E>,
    _phantom_v: PhantomData<V>,
}
impl<S, V, E> GraphStorage<V, E> for UGraph<S, V, E>
where
    S: GraphStorage<V, E>,
{
    fn add_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool> {
        let a = self.storage.add_edge(from, to, edge);
        let b = self.storage.add_edge(to, from, edge);

        if let Some(a) = a {
            if let Some(b) = b {
                return Some(a && b);
            } else {
                return None;
            }
        } else {
            return None;
        }
    }

    fn edge_size(&self) -> u64 {
        self.storage.edge_size() as u64 / 2
    }
    delegate! {
        to self.storage {
            fn add_vertex(&mut self, vertex: &V) -> Option<bool>;
            fn remove_vertex(&mut self, vertex: &V) -> bool;
            fn remove_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool>;
            fn has_vertex(&self, vertex: &V) -> bool;
            fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool;
            fn neighbors(&self, vertex: &V) -> Option<Vec<(V, E)>>;
            fn set_edge(
                &mut self,
                from: &V,
                to: &V,
                old_edge: &E,
                new_edge: &E
            ) -> Option<bool>;
            fn set_vertex(&mut self, old_vertex: &V, new_vertex: &V) -> Option<bool>;

            fn vertex_size(&self) -> u64 ;


        }
    }
}

impl<S, V, E> FixedStorage for UGraph<S, V, E>
where
    S: GraphStorage<V, E> + FixedStorage,
{
    fn new(size: u32) -> Self {
        UGraph {
            storage: S::new(size),
            _phantom_e: PhantomData,
            _phantom_v: PhantomData,
        }
    }
}

impl<S, V, E> DynamicStorage for UGraph<S, V, E>
where
    S: GraphStorage<V, E> + DynamicStorage,
{
    fn new() -> Self {
        UGraph {
            storage: S::new(),
            _phantom_e: PhantomData,
            _phantom_v: PhantomData,
        }
    }
}
#[cfg(test)]
mod u_graph {

    #[test]
    fn test() {}
}
