import numpy as np  # for vector operations

from scipy.io import mmread

from scipy.sparse import tril, triu
from scipy.sparse import csgraph


def sp_graph_from_mm_file(path):
    return mmread(path).tocsr()


def sp_bfs(graph, src_vertex):
    """
    Computes traversal level for each vertex from source vertex.

    graph: any graph
    src_vertex: source vertex

    return: Vector of visited vertices
    """
    n_verts = graph.get_shape()[0]

    res_vect = np.zeros(n_verts)
    found_nodes_vect = np.zeros(n_verts, dtype=bool)
    found_nodes_vect[src_vertex] = True

    not_empty = True
    level = 1

    while not_empty and level <= n_verts:
        for i in range(n_verts):
            if (found_nodes_vect[i]):
                res_vect[i] = level

        # update mask to only find the next undiscovered nodes
        found_nodes_vect = ((res_vect @ graph > 0) - res_vect) > 0

        not_empty = found_nodes_vect.sum() > 0
        level += 1

    return res_vect


def sp_triangle_count(graph):
    """
    Computes the number of triangles in the graph.

    graph: undirected graph

    return: (int) number of triangles
    """
    def triangular_adj_matr_count(adj_matrix_part):
        res_matr = adj_matrix_part.multiply(adj_matrix_part*adj_matrix_part)
        return int(res_matr.sum())

    lower_matr = tril(graph)
    upper_matr = triu(graph)

    # check if graph undirected
    if (lower_matr != upper_matr.transpose()).nnz == 0:
        count = triangular_adj_matr_count(lower_matr)
        return count

    raise Exception("Graph is not undirected")


def sp_bellman_ford(graph, src_vertex):
    """
    From a given start vertex, finds the shortest paths to every other
    (reachable) vertex in the graph.

    graph: weighted graph
    src_vertex: source vertex

    return: Vector of computed distances
    """
    return csgraph.bellman_ford(graph,
                                indices=src_vertex,
                                return_predecessors=False)
