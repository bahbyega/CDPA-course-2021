import os.path
import pytest

from pygraphblas import Vector

from src.hw_2.Graphs import UnweightedGraph
from src.hw_2.algorithms import *


test_data_path = os.path.join(os.getcwd(), 'tests/test_data/')


# BFS tests
def test_Graph_perform_level_BFS_on_basic_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_basic_graph.txt'))

    actual = perform_level_BFS(graph, 0)
    expected = Vector.from_list([1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)

    print(all([act_item == exp_item for act_item,
          exp_item in zip(actual, expected)]))


def test_Graph_perform_level_BFS_on_disconnected_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_disconnected_graph.txt'))

    actual = perform_level_BFS(graph, 1)
    expected = Vector.from_list([0, 1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)

    print(all([act_item == exp_item for act_item,
          exp_item in zip(actual, expected)]))


# Triangle count tests
def test_Graph_perfrom_triangles_count_on_undir_graph():
    graph = UnweightedGraph.load_from_file(os.path.join(
        test_data_path, 'small_undir_graph.txt'))

    actual = perform_triangles_count_on_undir_graph(graph)
    expected = 5

    assert actual == expected
