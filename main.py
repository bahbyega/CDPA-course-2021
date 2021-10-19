from os import path, getcwd
from argparse import ArgumentParser

from src.hw_2.Graphs import UnweightedGraph, WeightedGraph
from src.hw_2.grb_algorithms import *
from src.hw_2.scipy_algorithms import *
from src.hw_2.std_algorithms import *


def main():
    parser = ArgumentParser(description='Perform algorithms on graphs')

    parser.add_argument('graph_filepath',
                        help='Path to your graph.mtx file in MatrixMarket format')

    parser.add_argument('-w', '--weighted', action='store_true',
                        help='Specify this flag if graph is weighted')

    parser.add_argument('--blas', action='store_true',
                        help='Specify this flag if you want to run algorithms with pygraphblas')

    parser.add_argument('--scipy', action='store_true',
                        help='Specify this flag if you want to run algorithms with scipy')

    parser.add_argument('--std', action='store_true',
                        help='Specify this flag if you want to run algorithms with standard library')

    algorithms_choices = ['level_bfs', 'triangles_count', 'bellman_ford']
    parser.add_argument('-p', '--perform', choices=algorithms_choices,
                        help='algorithm to perform: level_bfs, triangles_count, bellman_ford')

    args = parser.parse_args()

    if args.weighted:
        graph = WeightedGraph.from_mm_file(args.graph_filepath)
    else:
        graph = UnweightedGraph.from_mm_file(args.graph_filepath)

    graph_full_filepath = path.join(getcwd(), args.graph_filepath)
    print(f'\nGraph from {graph_full_filepath} :\n{graph.matrix}\n')

    if args.perform is not None:
        if 'level_bfs' in args.perform:
            src_vertex = int(input('Specify source vertex for Level BFS: '))
            if args.scipy:
                graph = sp_graph_from_mm_file(graph_full_filepath)
                print(f'\nLevel BFS:\n{sp_bfs(graph, src_vertex)}')
            elif args.std:
                graph = std_dir_graph_from_mm_file(graph_full_filepath)
                print(f'\nLevel BFS:\n{std_bfs(graph, src_vertex)}')
            else:
                print(f'\nLevel BFS:\n{perform_level_bfs(graph, src_vertex)}')

        if 'triangles_count' in args.perform:
            if args.scipy:
                graph = sp_graph_from_mm_file(graph_full_filepath)
                print(f'Triangles count: {sp_triangle_count(graph)}')
            elif args.std:
                graph = std_undir_graph_from_mm_file(graph_full_filepath)
                print(f'Triangles count: {std_triangles_count(graph)}')
            else:
                print(f'Triangles count: {perform_triangles_count(graph)}')

        if 'bellman_ford' in args.perform:
            src_vertex = int(input('Specify source vertex for Bellman-Ford: '))
            if args.scipy:
                graph = sp_graph_from_mm_file(graph_full_filepath)
                print(
                    f'\nSSSP (Bellman-Ford):\n{sp_bellman_ford(graph, src_vertex)}')
            elif args.std:
                graph = std_dir_graph_from_mm_file(graph_full_filepath)
                print(
                    f'\nSSSP (Bellman-Ford):\n{std_bellman_ford(graph, src_vertex)}')
            else:
                print(
                    f'\nSSSP (Bellman-Ford):\n{perform_bellman_ford(graph, src_vertex)}')


if __name__ == "__main__":
    main()
