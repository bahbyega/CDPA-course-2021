from math import inf

from pygraphblas import (
    BOOL,
    INT64,
    FP64,
    Matrix,
    Vector,
    semiring,
    descriptor
)


def perform_level_bfs(graph, src_vertex):
    """
    Computes traversal level for each vertex from source vertex.

    graph: any graph
    src_vertext: source vertex

    return: Vector of visited vertices
    """
    res_vect = Vector.dense(INT64, graph.num_verts, fill=0)
    found_nodes_vect = Vector.sparse(BOOL, graph.num_verts)
    found_nodes_vect[src_vertex] = True

    not_empty = True
    level = 1

    while not_empty and level <= graph.num_verts:
        res_vect.assign_scalar(level, mask=found_nodes_vect)

        with semiring.LOR_LAND_BOOL:
            found_nodes_vect = res_vect.vxm(
                graph.matrix, mask=res_vect, desc=descriptor.RC)

        not_empty = found_nodes_vect.reduce_bool()
        level += 1

    return res_vect


def perform_triangles_count(graph):
    """
    Computes the number of triangles in the graph.

    graph: undirected graph

    return: (int) number of triangles
    """
    # takes lower or upper triangular portion of adj matrix
    def triangular_adj_matr_count(adj_matrix_part):
        res_matr = Matrix.sparse(INT64, graph.num_verts, graph.num_verts)

        with semiring.PLUS_TIMES_INT64:
            res_matr = adj_matrix_part.mxm(
                adj_matrix_part, mask=adj_matrix_part)

        return res_matr.reduce_int()

    lower_matr = graph.matrix.tril()
    upper_matr = graph.matrix.triu()

    # check if graph undirected
    if (lower_matr.iseq(upper_matr.transpose())):
        count = triangular_adj_matr_count(lower_matr)
        return count

    raise Exception("Graph is not undirected")


def perform_bellman_ford(graph, src_vertex):
    """
    From a given start vertex, finds the shortest paths to every other
    (reachable) vertex in the graph.

    graph: weighted graph
    src_vertext: source vertex

    return: Vector of computed distances
    """
    res_vect = Vector.dense(FP64, graph.num_verts, fill=inf)
    res_vect[src_vertex] = 0.0

    with semiring.MIN_PLUS_FP64:
        for _ in range(graph.num_verts):
            found_vect = res_vect.dup()
            res_vect = res_vect.vxm(
                graph.matrix, accum=FP64.MIN)

            if found_vect.iseq(res_vect):
                break

    return res_vect
