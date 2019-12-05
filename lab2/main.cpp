#include "random"
#include "map"
#include "chrono"
#include "omp.h"

#define ARRAY_SIZE 15000
#define REQUIRED_NUMBER 42

int *create_array(int size) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    int *array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = dist(mt);
    }

    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d, ", array[i]);
        if (i % 20 == 0) {
            printf("\n");
        }
    }
    printf("] \n");
    return array;
}

std::map<int, int> count_numbers(int *array, int size) {
    std::map<int, int> result;
    for (int i = 0; i < size; ++i) {
        auto pair = result.find(array[i]);
        if (pair != result.end()) {
            result.erase(array[i]);
            result.insert(std::pair<int, int>(array[i], pair->second + 1));
        } else {
            result.insert(std::pair<int, int>(array[i], 1));
        }
    }
    return result;
}


void fix_map(std::map<int, int> *map, int max_value) {
    for (auto it = map->cbegin(); it != map->cend();) {
        if (it->first >= max_value || it->first <= 0) {
            map->erase(it++);
        } else {
            ++it;
        }
    }
}

int solve(std::map<int, int> *map, int required_value, int elementsInMap) {
    int packs = 0;
    int value = required_value;
#pragma omp parallel for
    for (long i = 0; i < elementsInMap; i++) {
        value = value - (--map->end())->first;
        (--map->end())->second = (--map->end())->second - 1;
#pragma omp critical
        if ((--map->end())->second == 0) {
            map->erase((--map->end())->first);
        }
        if (value - map->begin()->first == 0) {
            packs++;
            value = required_value;
            map->begin()->second = map->begin()->second - 1;
#pragma omp critical
            if (map->begin()->second == 0) {
                map->erase(map->begin()->first);
            }
        } else {
            if (map->begin()->first == value) {
                value = required_value;
                packs++;
                map->begin()->second = map->begin()->second - 1;
#pragma omp critical
                if (map->begin()->second == 0) {
                    map->erase(map->begin()->first);
                }
                continue;
            } else if (map->begin()->first < value) {
                int initial = value;
                for (int j = 0; j < initial; ++j) {
                    value = value - map->begin()->first;
                    map->begin()->second = map->begin()->second - 1;
#pragma omp critical
                    if (map->begin()->second == 0) {
                        map->erase(map->begin()->first);
                    }
                    if (value == 0) {
                        packs++;
                        value = required_value;
                        continue;
                    } else if (value < 0) {
                        value = required_value;
                        continue;
                    }
                }
            } else {
                value = required_value;
            }
        }
    }

    return packs;
}


int main() {
    int *given_array = create_array(ARRAY_SIZE);
    std::map<int, int> numbers = count_numbers(given_array, ARRAY_SIZE);
    fix_map(&numbers, REQUIRED_NUMBER);

    int elements_in_map = 0;
    printf("\n Pairs for value search:\n");
    for (auto i = numbers.begin(); i != numbers.end(); i++) {
        printf("Key: %d, Count: %d\n", i->first, i->second);
        elements_in_map += i->second;
    }

    printf("In map %d elements \n", elements_in_map);

    auto exec_start_time = std::chrono::high_resolution_clock::now();

    int subsets = solve(&numbers, REQUIRED_NUMBER, elements_in_map);

    auto exec_stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(exec_stop_time - exec_start_time).count();

    printf("Found %d subsets\n", subsets);
    printf("Took %lldl microseconds", duration);

    numbers.clear();
    delete given_array;
    return 0;
}