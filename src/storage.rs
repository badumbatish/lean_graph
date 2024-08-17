use std::vec::Vec;

pub trait DynamicStorage {
    fn new() -> Self;
}
pub trait FixedStorage {
    fn new(size: u32) -> Self;
}
pub trait GraphStorage<V, E> {
    /**
    Returns the number of edges in the graph

    This is direction-sensitive, so if there is an edge from A to B, and an edge from B to A, this function will return 2

    For undirected graphs, this function should return GraphStorage.edge_size() / 2

    */
    fn edge_size(&self) -> u64;
    fn vertex_size(&self) -> u64;

    /**
        If vertex is already in graph, returns false.

        If vertex is not in graph and still has capacity, returns true

        If vertex is not in graph and has no capacity, returns None

        Please unwrap immediately after calling this function
    */
    fn add_vertex(&mut self, vertex: &V) -> Option<bool>;

    /**
        If vertex is already in graph, changes and return true

        If vertex is not in graph, returns false

        Any operation that is unable to do the two things above returns None
    */
    fn set_vertex(&mut self, old_vertex: &V, new_vertex: &V) -> Option<bool>;

    /// If vertex is in graph, removes and return true
    /// If vertex is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn remove_vertex(&mut self, vertex: &V) -> bool;

    /// If edge is already in graph, returns false
    /// If edge is not in graph and still has capacity, returns true
    /// If edge is not in graph and has no capacity, returns None
    fn add_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool>;

    /// If edge is already in graph, remove and return true
    /// If edge is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn remove_edge(&mut self, from: &V, to: &V, edge: &E) -> Option<bool>;

    /// If vertex is in graph, returns true
    /// If vertex is not in graph, returns false
    fn has_vertex(&self, vertex: &V) -> bool;

    /// If edge is in graph, change edge and returns true
    /// If edge is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn set_edge(&mut self, from: &V, to: &V, old_edge: &E, new_edge: &E) -> Option<bool>;
    fn has_edge(&self, from: &V, to: &V, edge: &E) -> bool;
    fn neighbors(&self, vertex: &V) -> Option<Vec<(V, E)>>;
}

#[cfg(test)]
mod fixed_storage {}

#[cfg(test)]
mod dynamic_storage {}
