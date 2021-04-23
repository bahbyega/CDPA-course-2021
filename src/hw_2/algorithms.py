from pygraphblas import (
    BOOL,
    INT64,
    Matrix,
    Vector,
    semiring,
    descriptor
)


def perform_level_BFS(graph, src_vertex):
    res_vect = Vector.dense(INT64, graph.num_verts, fill=0)
    found_nodes_vect = Vector.sparse(BOOL, graph.num_verts)

    found_nodes_vect[src_vertex] = True
    found_not_empty = True
    level = 1

    while found_not_empty and level <= graph.num_verts:
        res_vect.assign_scalar(level, mask=found_nodes_vect)

        with semiring.LOR_LAND_BOOL:
            found_nodes_vect = res_vect.vxm(
                graph.matrix, mask=res_vect, desc=descriptor.RC)

        found_not_empty = found_nodes_vect.reduce_bool()
        level += 1

    return res_vect


def perform_triangles_count_on_undir_graph(graph):

    def triangular_adj_matr_count(adj_matrix_part):
        res_matr = Matrix.sparse(INT64, graph.num_verts, graph.num_verts)

        with semiring.PLUS_TIMES_INT64:
            res_matr = adj_matrix_part.mxm(
                adj_matrix_part, mask=adj_matrix_part, desc=descriptor.T1)

        return res_matr.reduce_int()

    lower_matr = graph.matrix.tril()
    upper_matr = graph.matrix.triu()

    # if graph undirected
    if (lower_matr.iseq(upper_matr.transpose())):
        count = triangular_adj_matr_count(lower_matr)
        return count

    raise Exception("Graph is not undirected")
