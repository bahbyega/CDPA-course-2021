from pygraphblas import (
    BOOL,
    FP64,
    Matrix
)


class UnweightedGraph:
    def __init__(self):
        self.matrix = Matrix.sparse(BOOL)

    def from_mm_file(path):
        with open(path, 'r') as f:
            graph = UnweightedGraph()
            graph.matrix = Matrix.from_mm(f, BOOL)

        return graph


class WeightedGraph:
    def __init__(self):
        self.matrix = Matrix.sparse(FP64)

    def from_mm_file(path):
        with open(path, 'r') as f:
            graph = WeightedGraph()
            graph.matrix = Matrix.from_mm(f, FP64)

        return graph
