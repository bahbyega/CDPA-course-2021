import os
from argparse import ArgumentParser

from src.hw_2.Graphs import *
from src.hw_2.grb_algorithms import *


def main():
    parser = ArgumentParser(description='Perform algorithms on graphs')

    parser.add_argument('graph_filepath',
                        help='Path to your graph.txt file, '
                        'where first line contains the number of vertices, '
                        'other lines contain information about edges '
                        'in the format of "src_vertex to_vertex [value]"')

    parser.add_argument('-w', '--weighted', action='store_true',
                        help='Specify this flag if graph is weighted')

    algorithms_choices = ['level_bfs', 'triangles_count', 'bellman_ford']
    parser.add_argument('-p', '--perform', choices=algorithms_choices,
                        help='algorithm to perform: level_bfs, triangles_count, bellman_ford')

    args = parser.parse_args()

    if args.weighted:
        graph = WeightedGraph.load_from_file(args.graph_filepath)
    else:
        graph = UnweightedGraph.load_from_file(args.graph_filepath)

    graph_full_filepath = os.path.join(os.getcwd(), args.graph_filepath)
    print(f'\nGraph from {graph_full_filepath} :\n{graph.matrix}\n')

    if args.perform is not None:
        if 'level_bfs' in args.perform:
            src_vertex = int(input('Specify source vertex for Level BFS: '))
            print(f'\nLevel BFS:\n{perform_level_bfs(graph, src_vertex)}')

        if 'triangles_count' in args.perform:
            print(f'Triangles count: {perform_triangles_count(graph)}')

        if 'bellman_ford' in args.perform:
            src_vertex = int(input('Specify source vertex for Bellman-Ford: '))
            print(
                f'\nSSSP (Bellman-Ford):\n{perform_bellman_ford(graph, src_vertex)}')


if __name__ == "__main__":
    main()
