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

To install gtk3 on debian based distros run 
```
sudo apt install libgtk-3-dev
```
Most likely OpenMP is already installed by default and will compile with gcc. Although, if it's not the case install it with: `sudo apt install libomp-dev`

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
$ conda create -q -n test-env python=3.8 pyghraphblas pytest
$ conda activate test-env
```
Run tests:
```
$ python -m pytest tests/test_small_graphs.py
```

### Task 2.1

> Необходимо выбрать и реализовать минимум 3 различных алгоритма (требующих различных операций) анализа графов с использованием операций линейной алгебры на (py)graphblas.

---