import os.path
import pytest

from pygraphblas import Vector

from src.hw_2.Graph import Graph


def test_Graph_perform_level_BFS_on_basic_graph():
    test_data_path = os.path.join(os.getcwd(), 'tests/test_data/')
    graph = Graph.load_from_file(os.path.join(
        test_data_path, 'small_basic_graph.txt'))

    actual = graph.perform_level_BFS(0)
    expected = Vector.from_list([1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)

    print(all([act_item == exp_item for act_item,
          exp_item in zip(actual, expected)]))


def test_Graph_perform_level_BFS_on_disconnected_graph():
    test_data_path = os.path.join(os.getcwd(), 'tests/test_data/')
    graph = Graph.load_from_file(os.path.join(
        test_data_path, 'small_disconnected_graph.txt'))

    actual = graph.perform_level_BFS(1)
    expected = Vector.from_list([0, 1, 2, 3, 2, 3, 4, 3])

    assert len(actual) == len(expected)
    assert actual.iseq(expected)

    print(all([act_item == exp_item for act_item,
          exp_item in zip(actual, expected)]))
