"""
Standard algorithms for graphs with networkx library
"""

from scipy.io import mmread  # only for loading mtx files
import networkx as nx


def std_undir_graph_from_mm_file(path):
    return nx.from_scipy_sparse_matrix(mmread(path))


def std_dir_graph_from_mm_file(path):
    return nx.from_scipy_sparse_matrix(mmread(path), create_using=nx.DiGraph)


def std_bfs(graph, src_vertex):
    """
    Computes traversal level for each vertex from source vertex.

    graph: any graph
    src_vertex: source vertex

    return: Vector of distances to visited vertices
    """
    # this sssp yields (node, level) in a breadth first search
    res = nx.single_source_shortest_path_length(graph, src_vertex)

    return [dist+1 for _, dist in sorted(res.items())]


def std_triangles_count(graph):
    """
    Computes the number of triangles in the graph.

    graph: undirected graph

    return: (int) number of triangles
    """
    if nx.is_directed(graph):
        raise Exception("Graph is not undirected")

    return sum(nx.triangles(graph).values()) // 3


def std_bellman_ford(graph, src_vertex):
    """
    From a given start vertex, finds the shortest paths to every other
    (reachable) vertex in the graph.

    graph: weighted graph
    src_vertex: source vertex

    return: Vector of computed distances
    """
    res = nx.single_source_bellman_ford_path_length(graph, src_vertex)

    return [dist for _, dist in sorted(res.items())]
