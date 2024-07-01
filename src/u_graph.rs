use crate::storage::GraphStorage;
use std::hash::Hash;
pub struct UGraph<V, E, S>
where
    V: Eq + Hash + Clone,
    E: Eq + Hash + Clone,
    S: GraphStorage<V, E>,
{
    storage: S,
    _phantom: std::marker::PhantomData<(V, E)>,
}
