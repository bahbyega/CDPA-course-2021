import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from matplotlib.ticker import ScalarFormatter
from matplotlib.ticker import FormatStrFormatter


def draw_bar(file, label, axs, position, bar_height):
    '''
    Draws bar chart for information in benchmark result file
    '''
    df = pd.read_csv(file)

    alg_means = list(df.mean(axis=0))
    plt.yticks([r + bar_height for r in range(len(alg_means))],
               ['CPU', 'CPU_p', 'GPU'])

    axs.barh(position, alg_means, label=label,
             height=bar_height, edgecolor='white', alpha=0.8)

    plt.ioff()


def draw_benchmarks(file_list, title, out, logscale):
    '''
    Draws benchmark results from csv files provided with file_list
    '''
    fig = plt.figure()
    axs = plt.axes()

    bar_height = 0.21
    position = np.arange(len(file_list) - 1)

    for (file, label) in file_list:
        draw_bar(file, label, axs, position, bar_height)
        position = [y + bar_height for y in position]

    # customizing chart
    if logscale:
        axs.set_xscale('log', base=2)
    axs.xaxis.set_major_formatter(ScalarFormatter())
    axs.xaxis.set_major_formatter(FormatStrFormatter('%.2f'))
    axs.set_xlabel('Execution time(sec)')
    axs.set_ylabel('Implementation')
    axs.xaxis.grid(True, zorder=0)
    axs.set_axisbelow(True)
    axs.minorticks_off()
    axs.legend(title='Size', loc=1)

    plt.title(title)
    plt.savefig(out)
    plt.close(fig)


if __name__ == "__main__":
    draw_benchmarks([('../../../benchmarks/ImageFilteringApp/output/5/640x246.csv', '640x246'),
                    ('../../../benchmarks/ImageFilteringApp/output/5/1280x853.csv', '1280x853'),
                    ('../../../benchmarks/ImageFilteringApp/output/5/1920x1280.csv', '1920x1280'),
                    ('../../../benchmarks/ImageFilteringApp/output/5/5184x3456.csv', '5184x3456')],
                    '5x5 kernel', '5x5.pdf', True)

    draw_benchmarks([('../../../benchmarks/ImageFilteringApp/output/9/640x246.csv', '640x246'),
                    ('../../../benchmarks/ImageFilteringApp/output/9/1280x853.csv', '1280x853'),
                    ('../../../benchmarks/ImageFilteringApp/output/9/1920x1280.csv', '1920x1280'),
                    ('../../../benchmarks/ImageFilteringApp/output/9/5184x3456.csv', '5184x3456')],
                    '9x9 kernel', '9x9.pdf', True)

    draw_benchmarks([('../../../benchmarks/ImageFilteringApp/output/3/1280x853.csv', '3x3'),
                    ('../../../benchmarks/ImageFilteringApp/output/5/1280x853.csv', '5x5'),
                    ('../../../benchmarks/ImageFilteringApp/output/9/1280x853.csv', '9x9'),
                    ('../../../benchmarks/ImageFilteringApp/output/15/1280x853.csv', '15x15')],
                    'Image 1280x853px with different filters', '1280x853.pdf', True)
