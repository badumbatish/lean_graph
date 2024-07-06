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

    fn ne(&self, other: &Self) -> bool {
        !self.eq(other)
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
    fn add_vertex(&mut self, vertex: &Vertex) -> bool;
    fn set_vertex(&mut self, old_vertex: &Vertex, new_vertex: &Vertex) -> bool;
    fn remove_vertex(&mut self, vertex: &Vertex) -> bool;
    fn add_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool;
    fn remove_edge(&mut self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool;
    fn has_vertex(&self, vertex: &Vertex) -> bool;
    fn set_edge(
        &mut self,
        from: &Vertex,
        to: &Vertex,
        old_edge: &crate::storage::Edge,
        new_edge: &crate::storage::Edge,
    ) -> bool;
    fn has_edge(&self, from: &Vertex, to: &Vertex, edge: &Edge) -> bool;
    fn neighbors(&self, vertex: &Vertex) -> Vec<(Vertex, Edge)>;
}
