use std::hash::{Hash, Hasher};
use std::vec::Vec;

#[derive(Debug, Clone)]
pub struct Vertex {
    pub name: String,
    pub weight: f32,
}
impl Hash for Vertex {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.name.hash(state);
    }
}
impl PartialEq for Vertex {
    fn eq(&self, other: &Self) -> bool {
        self.name == other.name
    }
}

impl Eq for Vertex {}

#[derive(Debug, Clone)]
pub struct Edge {
    pub name: String,
    pub weight: f32,
}

impl Hash for Edge {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.name.hash(state);
    }
}
impl PartialEq for Edge {
    fn eq(&self, other: &Self) -> bool {
        self.name == other.name
    }

    fn ne(&self, other: &Self) -> bool {
        !self.eq(other)
    }
}

impl Eq for Edge {}

pub trait DynamicStorage {
    fn new() -> Self;
}
pub trait FixedStorage {
    fn new(size: u32) -> Self;
}
pub trait GraphStorage {
    fn edge_size(&self) -> u64;
    fn vertex_size(&self) -> u64;

    /**
        If vertex is already in graph, returns false.

        If vertex is not in graph and still has capacity, returns true

        If vertex is not in graph and has no capacity, returns None

        Please unwrap immediately after calling this function
    */
    fn add_vertex(&mut self, vertex: &Vertex) -> Option<bool>;

    /**
        If vertex is already in graph, changes and return true

        If vertex is not in graph, returns false

        Any operation that is unable to do the two things above returns None
    */
    fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> Option<bool>;

    /// If vertex is in graph, removes and return true
    /// If vertex is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn remove_vertex(&mut self, vertex: &Vertex) -> bool;

    /// If edge is already in graph, returns false
    /// If edge is not in graph and still has capacity, returns true
    /// If edge is not in graph and has no capacity, returns None
    fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool>;

    /// If edge is already in graph, remove and return true
    /// If edge is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> Option<bool>;

    /// If vertex is in graph, returns true
    /// If vertex is not in graph, returns false
    fn has_vertex(&self, vertex: &Vertex) -> bool;

    /// If edge is in graph, change edge and returns true
    /// If edge is not in graph, returns false
    /// Any operation that is unable to do the two things above returns None
    fn set_edge(
        &mut self,
        from: &Vertex,
        to: &Vertex,
        old_edge: &crate::storage::Edge,
        new_edge: &crate::storage::Edge,
    ) -> Option<bool>;
    fn has_edge(&self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool;
    fn neighbors(&self, vertex: &Vertex) -> Option<Vec<(Vertex, Edge)>>;
}

#[cfg(test)]
mod fixed_storage {}

#[cfg(test)]
mod dynamic_storage {}
