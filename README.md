# Computer Data Processing Algorithms

This repository is for the homework assignments for the Computer Data Processing Algorithms course at St. Petersburg University, spring 2021.

---

## Homework I
[![Build Status](https://travis-ci.com/bahbyega/CDPA-course-2021.svg?branch=Task-1.1)](https://travis-ci.com/bahbyega/CDPA-course-2021)


### Requirements
First, make sure the following is installed on your machine:
- `gcc` compiler :)
- `gtk3` library for GUI
- `OpenMP` library for parallelism
- `OpenCL` for operations with GPU

To install gtk3 on debian based distros run 
```
sudo apt install libgtk-3-dev
```
Most likely OpenMP is already installed by default and will compile with gcc. Although, if it's not the case install it with: `sudo apt install libomp-dev`
OpenCL installation is different with each graphics card manufacturer. Although, you still need the following headers to compile the application:
```
$ sudo apt -y install opencl-headers
$ sudo apt -y install ocl-icd-opencl-dev
```

### Build
To build the application simply do
```
$ ./build.sh <build target (optional)>
```
### Build targets
- `Clean` - removes all the object files that had been created, so you could do the fresh build.

### Run
After you built the application you can run it with:
```
$ ./src/hw-1/build/out
```

*Note: run all these commands from root directory.*

### Task 1.1

> Реализовать приложение с графическим интерфейсом пользователя, позволяющее открыть папку с изображениями, выбрать изображение, просмотреть его, просмотреть информацию о нём (размер в пикселях, размер в мегабайтах).

### Task 1.2

> Расширить приложение графической компонентой задания матричного фильтра. Необходимо предусмотреть возможность выбора типа фильтра, дефолтных значений, размера фильтра, корректировку весов.

### Task 1.3

> Расширить приложение возможностью отображать одновременно два изображения: до и после применения фильтра. Предусмотреть возможность сохранять результат применения фильтра.


### Task 1.4
> Реализовать применение матричных фильтров с использованием GPGPU. Интегрировать с разработанным графическим интерфейсом. Предусмотреть возможность применения нескольких фильтров последовательно.

### Task 1.5
> Расширить разрабатываемое приложение возможностью потоковой обработки изображений: выбираем папку с изображениями и ко всем применяем заданные фильтры. Результаты применения фильтров сохраняются в отдельную выбранную папку.

### Task 1.6
> Подготовить отчёт с анализом производительности и масштабируемости полученного решения.

---

## Homework II
[![Build Status](https://www.travis-ci.com/bahbyega/CDPA-course-2021.svg?branch=Task-2.1)](https://www.travis-ci.com/bahbyega/CDPA-course-2021)

### Requirements
- `Conda` package manager is required for installing python packages. You can install it via [Miniconda](https://docs.conda.io/en/latest/miniconda.html).

### Executing
Add package channel for `pygraphblas`:
```
$ conda config --append channels conda-forge
```
Activate testing environment:
```
$ conda create -q -n test-env python=3.8 pyghraphblas pytest scipy networkx
$ conda activate test-env
```
Run program:
```
usage: main.py [-h] [-w] [--blas] [--scipy] [--std]
               [-p {level_bfs,triangles_count,bellman_ford}]
               graph_filepath


Perform algorithms on graphs

positional arguments:
  graph_filepath        Path to your graph.mtx file in MatrixMarket format

optional arguments:
  -h, --help            show this help message and exit
  -w, --weighted        Specify this flag if graph is weighted
  --blas                Specify this flag if you want to run algorithms with
                        pygraphblas
  --scipy               Specify this flag if you want to run algorithms with
                        scipy
  --std                 Specify this flag if you want to run algorithms with
                        standard library
  -p {level_bfs,triangles_count,bellman_ford}, --perform {level_bfs,triangles_count,bellman_ford}
                        Algorithm to perform: level_bfs, triangles_count,
                        bellman_ford
```
Example:
```
Run Bellman-Ford algorithm implemented with pygraphblas on weighted graph from test_data: 
$ python main.py --blas -w -p bellman_ford tests/test_data/weighted_graph.mtx
```

### Testing
To manually run tests activate testing environment and run:
```
$ python -m pytest
```

### Task 2.1

> Необходимо выбрать и реализовать минимум 3 различных алгоритма (требующих различных операций) анализа графов с использованием операций линейной алгебры на (py)graphblas.

### Task 2.2

> Реализовать выбранные алгоритмы на sciPy.

### Task 2.3

> Реализовать выбранные алгоритмы на стандартной библиотеке для анализа графов.

### Task 2.4

> Сравнить производительность полученных реализаций, составить отчёт.

---