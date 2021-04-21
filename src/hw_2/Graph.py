from pygraphblas import (
    BOOL,
    INT64,
    Matrix,
    Vector,
    semiring,
    descriptor
)


class Graph:
    def __init__(self, num_verts):
        self.matrix = Matrix.sparse(BOOL, num_verts, num_verts)
        self.num_verts = num_verts

    def load_from_file(path):
        with open(path, 'r') as f:
            verts_count = int(f.readline())
            graph = Graph(verts_count)

            for line in f:
                from_vert, to_vert = [
                    int(vert) for vert in line.replace('\n', '').split(' ', 1)]
                graph.matrix[from_vert, to_vert] = True

        return graph

    def perform_level_BFS(self, src_vertex):
        res_vect = Vector.dense(INT64, self.num_verts, fill=0)
        found_nodes_vect = Vector.sparse(BOOL, self.num_verts)

        found_nodes_vect[src_vertex] = True
        found_not_empty = True
        level = 1

        while found_not_empty and level <= self.num_verts:
            res_vect.assign_scalar(level, mask=found_nodes_vect)

            with semiring.LOR_LAND_BOOL:
                found_nodes_vect = res_vect.vxm(
                    self.matrix, mask=res_vect, desc=descriptor.RC)

            found_not_empty = found_nodes_vect.reduce_bool()
            level += 1

        return res_vect

    def perform_triangle_count(self):

        def triangle_count_lower(lower_matrix_part):
            mult_res = Matrix.sparse(INT64, self.num_verts, self.num_verts)

            with semiring.PLUS_TIMES_INT64:
                mult_res = lower_matrix_part.mxm(
                    lower_matrix_part, mask=lower_matrix_part, desc=descriptor.T1)

            return mult_res.reduce_int()

        lower_matr = self.matrix.tril()

        return triangle_count_lower(lower_matr)
