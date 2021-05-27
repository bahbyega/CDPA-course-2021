import os
import time

from random import choice

from src.hw_2.Graphs import UnweightedGraph, WeightedGraph
from src.hw_2.grb_algorithms import *
from src.hw_2.scipy_algorithms import *
from src.hw_2.std_algorithms import *


DATASETS_PATH = os.path.join(
    os.getcwd(), 'LABasedGraphAlgorithms/datasets/')
OUTPUT_PATH = os.path.join(
    os.getcwd(), 'LABasedGraphAlgorithms/output/')


def measure_grb_runtime(graph_unweighted, graph_weighted, src_vertex):
    """
    Measures time of algorithms implemented with pygraphblas

    return: list of runtimes (in seconds) of bfs, triangle_count, bellman_ford
    """
    bfs_start_time = time.monotonic()
    perform_level_bfs(graph_unweighted, src_vertex)
    bfs_run_time = time.monotonic() - bfs_start_time

    triangles_start_time = time.monotonic()
    perform_triangles_count(graph_unweighted)
    triangles_run_time = time.monotonic() - triangles_start_time

    bellford_start_time = time.monotonic()
    perform_bellman_ford(graph_weighted, src_vertex)
    bellford_run_time = time.monotonic() - bellford_start_time

    return [bfs_run_time, triangles_run_time, bellford_run_time]


def measure_sp_runtime(graph, src_vertex):
    """
    Measures time of algorithms implemented with SciPy

    return: list of runtimes (in seconds) of bfs, triangle_count, bellman_ford
    """
    bfs_start_time = time.monotonic()
    sp_bfs(graph, src_vertex)
    bfs_run_time = time.monotonic() - bfs_start_time

    triangles_start_time = time.monotonic()
    sp_triangle_count(graph)
    triangles_run_time = time.monotonic() - triangles_start_time

    bellford_start_time = time.monotonic()
    sp_bellman_ford(graph, src_vertex)
    bellford_run_time = time.monotonic() - bellford_start_time

    return [bfs_run_time, triangles_run_time, bellford_run_time]


def measure_std_runtime(graph, src_vertex):
    """
    Measures time of algorithms implemented with NetworkX

    return: list of runtimes (in seconds) of bfs, triangle_count, bellman_ford
    """
    bfs_start_time = time.monotonic()
    std_bfs(graph, src_vertex)
    bfs_run_time = time.monotonic() - bfs_start_time

    triangles_start_time = time.monotonic()
    std_triangles_count(graph)
    triangles_run_time = time.monotonic() - triangles_start_time

    bellford_start_time = time.monotonic()
    std_bellman_ford(graph, src_vertex)
    bellford_run_time = time.monotonic() - bellford_start_time

    return [bfs_run_time, triangles_run_time, bellford_run_time]


def graph_algorithms_benchmark():
    datasets = ['ca-AstroPh', 'ca-CondMat', 'ca-HepTh',
                'amazon-0302', 'amazon-0302.1', 'amazon-0312', 'amazon-0505', 'amazon-0601'
                'com-Youtube']
    count_of_iterations = 5

    for dataset in datasets:
        dataset_type = dataset.split('-')[0]

        with open(f'{OUTPUT_PATH}{dataset_type}/{dataset}_pygraphblas.csv', 'w+') as out_csv_pygraphblas,\
                open(f'{OUTPUT_PATH}{dataset_type}/{dataset}_scipy.csv', 'w+') as out_csv_scipy,\
                open(f'{OUTPUT_PATH}{dataset_type}/{dataset}_networkx.csv', 'w+') as out_csv_networkx:

            # field names
            out_csv_pygraphblas.write(
                'breadth_first_search,triangles_count,bellman_ford\n')
            out_csv_scipy.write(
                'breadth_first_search,triangles_count,bellman_ford\n')
            out_csv_networkx.write(
                'breadth_first_search,triangles_count,bellman_ford\n')

            graph_dir = f'{DATASETS_PATH}{dataset_type}/'
            graph_path = os.path.join(graph_dir, f'{dataset}.mtx')

            print(
                f'\n---------- Loading graphs for {dataset} ... ----------\n')

            grb_unweighted_graph = UnweightedGraph.from_mm_file(graph_path)
            grb_weighted_graph = WeightedGraph.from_mm_file(graph_path)
            sp_graph = sp_graph_from_mm_file(graph_path)
            std_graph = std_undir_graph_from_mm_file(graph_path)

            for i in range(1, count_of_iterations + 1):
                print(
                    f'Choosing source vertex for {dataset} ... {i} iteration')
                src_vertex = choice(list(std_graph.nodes()))

                print(
                    f'Running GraphBlas algorithms for {dataset} ... {i} iteration')
                grb_res = measure_grb_runtime(
                    grb_unweighted_graph, grb_weighted_graph, src_vertex)

                print(
                    f'Running SciPy algorithms for {dataset} ... {i} iteration')
                sp_res = measure_sp_runtime(sp_graph, src_vertex)

                print(
                    f'Running Networkx algorithms for {dataset} ... {i} iteration')
                std_res = measure_std_runtime(std_graph, src_vertex)

                print(
                    f'Writing results in output files for {dataset} ... {i} iteration')

                out_csv_pygraphblas.write(
                    f'{grb_res[0]},{grb_res[1]},{grb_res[2]}\n')
                out_csv_scipy.write(
                    f'{sp_res[0]},{sp_res[1]},{sp_res[2]}\n')
                out_csv_networkx.write(
                    f'{std_res[0]},{std_res[1]},{std_res[2]}\n')

                print(
                    f'---------- Iteration {i} for {dataset} finished ----------\n')

            print(
                f'----------Benchmark for {dataset} finished----------\n')
