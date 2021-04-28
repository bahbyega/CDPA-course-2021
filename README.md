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

### Task 1.4
> Реализовать применение матричных фильтров с использованием GPGPU. Интегрировать с разработанным графическим интерфейсом. Предусмотреть возможность применения нескольких фильтров последовательно.

---
