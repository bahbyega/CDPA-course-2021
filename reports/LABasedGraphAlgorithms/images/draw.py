import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import random as rnd

from matplotlib.ticker import ScalarFormatter
from matplotlib.ticker import FormatStrFormatter


def draw_bar(file, label, axs, position, bar_height, with_triangle_count):
    df = pd.read_csv(file)

    alg_means = list(df.mean(axis=0))
    if with_triangle_count:
        plt.yticks([r + bar_height for r in range(len(alg_means))],
                   ['bfs', 'tri', 'bford'])
    else:
        alg_means = alg_means[:1] + alg_means[2:]
        plt.yticks([r + bar_height for r in range(len(alg_means))],
                   ['bfs', 'bford'])

    axs.barh(position, alg_means, label=label,
             height=bar_height, edgecolor='white', alpha=0.8)

    plt.ioff()


def draw_benchmarks(file_list, title, out, with_triangle_count, logscale):
    fig = plt.figure()
    axs = plt.axes()

    # two types of plots
    if with_triangle_count:
        bar_height = 0.18
        position = np.arange(len(file_list))
    else:
        bar_height = 0.21
        position = np.arange(len(file_list) - 1)

    for (file, label) in file_list:
        draw_bar(file, label, axs, position, bar_height, with_triangle_count)
        position = [y + bar_height for y in position]

    # customizing chart
    if logscale:
        axs.set_xscale('log', base=2)
    axs.xaxis.set_major_formatter(ScalarFormatter())
    axs.xaxis.set_major_formatter(FormatStrFormatter('%.2f'))
    axs.set_xlabel('Время исполнения(сек)')
    axs.set_ylabel('Алгоритм')
    axs.xaxis.grid(True, zorder=0)
    axs.set_axisbelow(True)
    axs.minorticks_off()
    axs.legend(title='Library', loc=4)

    plt.title(title)
    plt.savefig(out)
    plt.close(fig)


if __name__ == "__main__":
    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-AstroPh_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-AstroPh_scipy.csv', 'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-AstroPh_networkx.csv', 'networkX')],
                    'ca-AstroPh (|V|=18,772; |E|=198,110)', 'ca-AstroPh.pdf', True, True)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-CondMat_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-CondMat_scipy.csv', 'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-CondMat_networkx.csv', 'networkX')],
                    'ca-CondMat (|V|=23,133; |E|=93,497)', 'ca-CondMat.pdf', True, True)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-HepTh_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-HepTh_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/ca/ca-HepTh_networkx.csv', 'networkX')],
                    'ca-HepTh (|V|=9,877; |E|=25,998)', 'ca-HepTh.pdf', True, True)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302_networkx.csv', 'networkX')],
                    'amazon-0302 (|V|=262,111; |E|=1,234,877)', 'amazon-0302.pdf', False, True)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302.1_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302.1_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0302.1_networkx.csv', 'networkX')],
                    'amazon-0302 (|V|=262,111; |E|=1,234,877)', 'amazon-0302.1.pdf', False, False)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0312_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0312_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0312_networkx.csv', 'networkX')],
                    'amazon-0312 (|V|=400,727; |E|=3,200,440)', 'amazon-0312.pdf', False, False)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0505_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0505_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0505_networkx.csv', 'networkX')],
                    'amazon-0505 (|V|=410,236; |E|=3,356,824)', 'amazon-0505.pdf', False, False)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0601_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0601_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/amazon/amazon-0601_networkx.csv', 'networkX')],
                    'amazon-0601 (|V|=403,394; |E|=3,387,388)', 'amazon-0601.pdf', False, False)

    draw_benchmarks([('../../../benchmarks/LABasedGraphAlgorithms/output/com/com-Youtube_pygraphblas.csv', 'pyGrb'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/com/com-Youtube_scipy.csv',  'sciPy'),
                    ('../../../benchmarks/LABasedGraphAlgorithms/output/com/com-Youtube_networkx.csv', 'networkX')],
                    'com-Youtube (|V|=1,134,890; |E|=2,987,624)', 'com-Youtube.pdf', True, True)
