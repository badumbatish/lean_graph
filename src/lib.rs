pub mod adjacency_list;
pub mod adjacency_matrix;
pub mod di_graph;
pub mod iterators;
pub mod storage;
pub mod u_graph;
pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
