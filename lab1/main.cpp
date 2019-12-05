#include <random>
#include "map"
#include "chrono"

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

int solve(std::map<int, int> *map, int required_value) {
    int packs = 0;
    int value = required_value;

    while (value != 0 && !map->empty()) {
        if (map->empty()) {
            return 0;
        }

        value = value - (--map->end())->first;
        (--map->end())->second = (--map->end())->second - 1;
        if ((--map->end())->second == 0) {
            map->erase((--map->end())->first);
        }
        int temp = value;
        bool all_values_checked = false;
        while (!all_values_checked) {
            auto may_be_last_value = map->find(temp);
            if (may_be_last_value != map->end()) {
                value = value - may_be_last_value->first;
                may_be_last_value->second = may_be_last_value->second - 1;
                if (may_be_last_value->second == 0) {
                    map->erase(may_be_last_value);
                }
                if (value == 0) {
                    packs++;
                    all_values_checked = true;
                    value = required_value;
                } else {
                    temp = value;
                }
            } else {
                if (temp < 0 || map->begin()->first > temp || value < 0) {
                    all_values_checked = true;
                    value = required_value;
                } else {
                    temp = temp - 1;
                }
            }
        }
    }

    return packs;
}

int main() {
    int *given_array = create_array(ARRAY_SIZE);
    std::map<int, int> numbers = count_numbers(given_array, ARRAY_SIZE);
    fix_map(&numbers, REQUIRED_NUMBER);

    printf("\n Pairs for value search:\n");
    int elements_in_map = 0;
    for (auto i = numbers.begin(); i != numbers.end(); i++) {
        printf("Key: %d, Count: %d\n", i->first, i->second);
        elements_in_map += i->second;
    }

    printf("In map %d elements \n", elements_in_map);

    auto exec_start_time = std::chrono::high_resolution_clock::now();

    int subsets = solve(&numbers, REQUIRED_NUMBER);

    auto exec_stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(exec_stop_time - exec_start_time).count();

    printf("Found %d subsets\n", subsets);
    printf("Took %lldl microseconds", duration);

    numbers.clear();
    delete given_array;
    return 0;
}