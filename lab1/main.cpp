#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <map>
#include <math.h>
#include <chrono>

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
    for (int i = 0; i < size; ++i) {
        pows[i] = (int) pow(2, i);
    }

    int s = 0;
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

    auto exec_start_time = std::chrono::high_resolution_clock::now();
    subsets(input, target, INPUT_SIZE);
    auto exec_stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(exec_stop_time - exec_start_time).count();

    printf("\nTook %lldl microseconds", duration);
    return 0;
}
