from pygraphblas import (
    BOOL,
    FP64,
    Matrix
)


class UnweightedGraph:
    def __init__(self, num_verts):
        self.matrix = Matrix.sparse(BOOL, num_verts, num_verts)
        self.num_verts = num_verts

    def load_from_file(path):
        with open(path, 'r') as f:
            verts_count = int(f.readline())
            graph = UnweightedGraph(verts_count)

            for line in f:
                from_vert, to_vert = [
                    int(vert) for vert in line.split(' ')[:2]]
                graph.matrix[from_vert, to_vert] = True

        return graph


class WeightedGraph:
    def __init__(self, num_verts):
        self.matrix = Matrix.sparse(FP64, num_verts, num_verts)
        self.num_verts = num_verts

    def load_from_file(path):
        with open(path, 'r') as f:
            verts_count = int(f.readline())
            graph = WeightedGraph(verts_count)

            for line in f:
                edge = line.split(' ', 2)[:3]
                from_vert, to_vert = int(edge[0]), int(edge[1])
                value = float(edge[2])

                graph.matrix[from_vert, to_vert] = value

        return graph
