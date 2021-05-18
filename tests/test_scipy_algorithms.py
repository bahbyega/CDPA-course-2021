import os.path
import pytest
import numpy as np

from src.hw_2.scipy_algorithms import *

TEST_DATA_PATH = os.path.join(os.getcwd(), 'tests/test_data/')


# BFS tests
def test_Graph_perform_level_bfs_on_basic_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.mtx'))

    actual = sp_bfs(graph, 0)
    expected = np.array([1, 2, 3, 2, 3, 4, 3])

    assert (actual == expected).all()


def test_Graph_perform_level_bfs_on_disconnected_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'disconnected_graph.mtx'))

    actual = sp_bfs(graph, 0)
    expected = np.array([1, 2, 3, 2, 3, 4, 3, 0])

    assert (actual == expected).all()


def test_Graph_perform_level_bfs_on_weighted_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.mtx'))

    actual = sp_bfs(graph, 0)
    expected = np.array([1, 2, 3, 2, 3, 4, 3])

    assert (actual == expected).all()


# Triangle count tests
def test_sp_triangles_count_on_undir_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'undir_graph.mtx'))

    actual = sp_triangle_count(graph)
    expected = 5

    assert actual == expected


def test_sp_triangles_count_on_triangleless_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'triangleless_graph.mtx'))

    actual = sp_triangle_count(graph)
    expected = 0

    assert actual == expected


def test_sp_triangles_count_on_dir_graph_raises_exception():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.mtx'))

    with pytest.raises(Exception) as excinfo:
        sp_triangle_count(graph)

    assert "Graph is not undirected" == str(excinfo.value)


# Bellman-ford tests
def test_sp_bellman_ford():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.mtx'))

    actual = sp_bellman_ford(graph, 0)
    expected = np.array([0.0, 0.3, 1.0, 0.8, 0.4, 0.5, 1.0])

    assert (actual == expected).all()


def test_sp_bellman_ford_on_negative_weighted_edges_graph():
    graph = sp_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'negative_weighted_graph.mtx'))

    actual = sp_bellman_ford(graph, 0)
    expected = np.array([0.0, 0.3, 0.8, 0.8, 0.4, 0.30000000000000004, 1.0])

    assert (actual == expected).all()
