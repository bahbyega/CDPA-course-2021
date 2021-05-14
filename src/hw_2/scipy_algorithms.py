from scipy.io import mmread

from scipy.sparse import csgraph as sp_graph
from scipy.sparse import csr_matrix as sp_matrix
from scipy.sparse import tril, triu


def sp_graph_from_mm_file(path):
    return mmread(path)


def sp_bfs(graph, src_vertex):
    # nocheckin: ordering, not level bfs
    res = sp_graph.breadth_first_order(graph, src_vertex, return_predecessors=False)
    return res

def sp_triangle_count(graph):
    def triangular_adj_matr_count(adj_matrix_part):
        res_matr = adj_matrix_part.multiply(adj_matrix_part*adj_matrix_part)
        return int(res_matr.sum())

    lower_matr = tril(graph)
    upper_matr = triu(graph)

    # check if graph undirected
    if (lower_matr!=upper_matr.transpose()).nnz==0:
        count = triangular_adj_matr_count(lower_matr)
        return count
    
    raise Exception("Graph is not undirected")
    

def sp_bellman_ford(graph, src_vertex):
    # nocheckin: is it ok to just call library func?
    res = sp_graph.bellman_ford(graph, indices=src_vertex, return_predecessors=False)
    return res