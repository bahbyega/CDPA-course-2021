import os.path
import pytest

from pygraphblas import Vector

from src.hw_2.Graphs import UnweightedGraph, WeightedGraph
from src.hw_2.grb_algorithms import *


TEST_DATA_PATH = os.path.join(os.getcwd(), 'tests/test_data/')


# BFS tests
def test_Graph_perform_level_bfs_on_basic_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.txt'))

    actual = perform_level_bfs(graph, 0)
    expected = Vector.from_list([1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


def test_Graph_perform_level_bfs_on_disconnected_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'disconnected_graph.txt'))

    actual = perform_level_bfs(graph, 1)
    expected = Vector.from_list([0, 1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


def test_Graph_perform_level_bfs_on_weighted_graph():
    graph = WeightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.txt'))

    actual = perform_level_bfs(graph, 0)
    expected = Vector.from_list([1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


# Triangle count tests
def test_Graph_perform_triangles_count_on_undir_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'undir_graph.txt'))

    actual = perform_triangles_count(graph)
    expected = 5

    assert actual == expected


def test_Graph_perform_triangles_count_on_triangleless_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'triangleless_graph.txt'))

    actual = perform_triangles_count(graph)
    expected = 0

    assert actual == expected


def test_Graph_perform_triangles_count_on_dir_graph_raises_exception():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.txt'))

    with pytest.raises(Exception) as excinfo:
        perform_triangles_count(graph)

    assert "Graph is not undirected" == str(excinfo.value)


# Bellman-ford tests
def test_Graph_perform_bellman_ford():
    graph = WeightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.txt'))

    actual = perform_bellman_ford(graph, 0)
    expected = Vector.from_list([0.0, 0.3, 1.0, 0.8, 0.4, 0.5, 1.0])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)


def test_Graph_perform_bellman_ford_on_unweigh_graph_raises_exception():
    graph = UnweightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.txt'))

    with pytest.raises(Exception) as excinfo:
        perform_bellman_ford(graph, 0)

    assert "Graph is not weighted" == str(excinfo.value)


def test_Graph_perform_bellman_ford_on_negative_weighted_edges_graph():
    graph = WeightedGraph.load_from_file(os.path.join(
        TEST_DATA_PATH, 'negative_weighted_graph.txt'))

    actual = perform_bellman_ford(graph, 0)
    expected = Vector.from_list(
        [0.0, 0.3, 0.8, 0.8, 0.4, 0.30000000000000004, 1.0])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)
