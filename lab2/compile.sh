#/bin/sh
rm omp
clang++ -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  main.cpp -o omp