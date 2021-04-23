import os.path
import pytest

from pygraphblas import Vector

from src.hw_2.Graphs import UnweightedGraph, WeightedGraph
from src.hw_2.algorithms import *


test_data_path = os.path.join(os.getcwd(), 'tests/test_data/')


# BFS tests
def test_Graph_perform_level_bfs_on_basic_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_basic_graph.txt'))

    actual = perform_level_bfs(graph, 0)
    expected = Vector.from_list([1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


def test_Graph_perform_level_bfs_on_disconnected_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_disconnected_graph.txt'))

    actual = perform_level_bfs(graph, 1)
    expected = Vector.from_list([0, 1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


# Triangle count tests
def test_Graph_perform_triangles_count_on_undir_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_undir_graph.txt'))

    actual = perform_triangles_count(graph)
    expected = 5

    assert actual == expected


# Bellman-ford tests
def test_Graph_perform_bellman_ford():
    graph = WeightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_weighted_graph.txt'))

    actual = perform_bellman_ford(graph, 0)
    expected = Vector.from_list([0.0, 0.3, 1.0, 0.8, 0.4, 0.5, 1.0])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)
