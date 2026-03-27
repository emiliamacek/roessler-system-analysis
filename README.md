## Requirements

The code used to carry out the proof and generate the data was written in C++ using the g++ (GCC) compiler version 15.1.1.
It utilizes the CAPD library version 6.0.0 (https://capd.ii.uj.edu.pl/), built from source.
The scripts for generating plots were written in Python version 3.13.3.

## Build Instructions

In the `Makefile`, you need to override the `capdconfig` variable with your path to the CAPD library. Next, to build the project, navigate to the main project directory in the terminal and run the `make` command. Once finished, all files from the `/src/` directory will be compiled and ready to execute. You can run them using the command `./program-name`.

## Code Navigation

- Implementation of functions related to the Poincaré map and the Rössler system:

```bash
/src/utils.cpp
```
- Lower bound:
```bash
/src/lower-bound.cpp
```
- Upper bound:
```bash
/src/upper-bound.cpp
```
- Generating points for the bifurcation diagram:
```bash
/src/poincare-bifurcation-diagram.cpp
```

## Performance
All procedures were tested on a machine running Arch Linux (kernel 6.15.2), equipped with an AMD Ryzen 5 5600 processor (6 cores / 12 threads, clock speed up to 4.47 GHz) and 32 GB of RAM.
