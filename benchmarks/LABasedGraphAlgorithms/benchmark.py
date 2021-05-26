import os
import time

from random import choice
from scipy.sparse import data

from src.hw_2.Graphs import UnweightedGraph, WeightedGraph
from src.hw_2.grb_algorithms import *
from src.hw_2.scipy_algorithms import *
from src.hw_2.std_algorithms import *


DATASETS_PATH = os.path.join(
    os.getcwd(), 'benchmarks/LABasedGraphAlgorithms/datasets/')
OUTPUT_PATH = os.path.join(
    os.getcwd(), 'benchmarks/LABasedGraphAlgorithms/output/')


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
    datasets = ['ca-AstroPh', 'ca-CondMat', 'ca-HepTh']

    algorithms = ['breadth_first_search', 'triangle_count', 'bellman_ford']

    for dataset in datasets:
        dataset_type = dataset.split('-')[0]
        with open(f'{OUTPUT_PATH}{dataset_type}/{dataset}.csv', 'w+') as out_csv:
            out_csv.write(
                f'dataset,algorithm_implementation,algorithm_name,algorithm_runtime\n')
            graph_dir = f'{DATASETS_PATH}{dataset_type}/'
            graph_path = os.path.join(graph_dir, f'{dataset}.mtx')

            print(
                f'\n---------- Loading graphs for {dataset} ... ----------\n')

            grb_unweighted_graph = UnweightedGraph.from_mm_file(graph_path)
            grb_weighted_graph = WeightedGraph.from_mm_file(graph_path)
            sp_graph = sp_graph_from_mm_file(graph_path)
            std_graph = std_undir_graph_from_mm_file(graph_path)

            count_of_iterations = 5

            for i in range(1, count_of_iterations + 1):
                print(
                    f'Choosing source vertex for {dataset} ... {i} iteration')
                src_vertex = choice(list(std_graph.nodes()))

                # run algorithms 3 times for each chosen vertex
                for _ in range(3):
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
                        f'Writing results in output file for {dataset} ... {i} iteration')
                    for j in range(3):
                        out_csv.write(
                            f'{dataset},pygraphblas,{algorithms[j]},{grb_res[j]}\n')
                        out_csv.write(
                            f'{dataset},scipy,{algorithms[j]},{sp_res[j]}\n')
                        out_csv.write(
                            f'{dataset},networkx,{algorithms[j]},{std_res[j]}\n')
                    print(
                        f'---------- Iteration {i} for {dataset} finished ----------\n')

            print(
                f'----------Benchmark for {dataset} finished----------\n')
