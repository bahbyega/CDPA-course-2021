import os.path
import pytest

from src.hw_2.std_algorithms import *


TEST_DATA_PATH = os.path.join(os.getcwd(), 'tests/test_data/')


# BFS tests
def test_std_nx_bfs_on_basic_graph():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.mtx'))

    actual = std_bfs(graph, 0)
    expected = iter([1, 2, 3, 2, 3, 4, 3])

    assert all(act_item == exp_item for act_item,
               exp_item in zip(actual, expected))


def test_std_nx_bfs_on_disconnected_graph():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'disconnected_graph.mtx'))

    actual = std_bfs(graph, 0)
    expected = iter([1, 2, 3, 2, 3, 4, 3, 0])

    assert all(act_item == exp_item for act_item,
               exp_item in zip(actual, expected))


def test_std_nx_bfs_on_weighted_graph():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.mtx'))

    actual = std_bfs(graph, 0)
    expected = iter([1, 2, 3, 2, 3, 4, 3])

    assert all(act_item == exp_item for act_item,
               exp_item in zip(actual, expected))


# Triangle count tests
def test_std_nx_triangles_count_on_undir_graph():
    graph = std_undir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'undir_graph.mtx'))

    actual = std_triangles_count(graph)
    expected = 5

    assert actual == expected


def test_std_nx_triangles_count_on_triangleless_graph():
    graph = std_undir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'triangleless_graph.mtx'))

    actual = std_triangles_count(graph)
    expected = 0

    assert actual == expected


def test_std_nx_triangles_count_on_dir_graph_raises_exception():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'basic_graph.mtx'))

    with pytest.raises(Exception) as excinfo:
        std_triangles_count(graph)

    assert "Graph is not undirected" == str(excinfo.value)


# Bellman-ford tests
def test_std_nx_bellman_ford():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'weighted_graph.mtx'))

    actual = std_bellman_ford(graph, 0)
    expected = list([0.0, 0.3, 1.0, 0.8, 0.4, 0.5, 1.0])

    assert all(act_item == exp_item for act_item,
               exp_item in zip(actual, expected))


def test_std_nx_bellman_ford_on_negative_weighted_edges_graph():
    graph = std_dir_graph_from_mm_file(os.path.join(
        TEST_DATA_PATH, 'negative_weighted_graph.mtx'))

    actual = std_bellman_ford(graph, 0)
    expected = iter([0.0, 0.3, 0.8, 0.8, 0.4, 0.30000000000000004, 1.0])

    assert all(act_item == exp_item for act_item,
               exp_item in zip(actual, expected))
