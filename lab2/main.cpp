#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <map>
#include <math.h>
#include <omp.h>

#define INPUT_SIZE 500

int *readSet(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) {
        throw std::runtime_error("Incorrect file path");
    }
    int *result = new int[INPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(fp, "%d", (result + i));
    }
    return result;
}

void subsets(int *numbers, int target, int size) {
    int *pows = new int[size];

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        pows[i] = (int) pow(2, i);
    }

    int s = 0;

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        if (pows[i] & target) {
            for (int j = 0; j < size; ++j) {
                if (pows[i] & numbers[j]) {
                    printf("%d ", numbers[j]);
                    s += numbers[j];
                    if (s >= target) {
                        printf("\n");
                        s = 0;
                    }
                }
            }
        }
    }

    delete[] pows;
}

/**
 * Run: ./out input_filename target
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        throw std::runtime_error("Run: ./out input_filename target");
    }

    int *input = readSet(argv[1]);
    int target = strtol(argv[2], NULL, 10);

    double exec_start_time = omp_get_wtime();
    subsets(input, target, INPUT_SIZE);
    double duration = omp_get_wtime() - exec_start_time;
    printf("\nTook %fl seconds", duration);
    return 0;
}
