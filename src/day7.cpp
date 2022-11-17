#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

#include "day7.h"

static void day7_run_p1() {
    std::ifstream file("../data/day7.txt");

    std::vector<int> values;
    while (file.good()) {
        int value;
        char ignored;

        file >> value >> ignored;
        values.push_back(value);
    }

    std::sort(values.begin(), values.end());
    int median = values[values.size() / 2];

    int result = 0;

    for (int value : values) {
        result += std::abs(value - median);
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day7_run_p2() {
    std::ifstream file("../data/day7.txt");

    std::vector<int> values;
    int total = 0;
    while (file.good()) {
        int value;
        char ignored;

        file >> value >> ignored;
        values.push_back(value);

        total += value;
    }

    int mean = total / values.size();

    int minimumFuel = std::numeric_limits<int>::max();
    for (int i = mean; i <= mean + 1; i++) {
        int fuel = 0;
        for (int value : values) {
            int dist = std::abs(value - i);
            fuel += (dist * dist + dist) / 2;
        }

        if (fuel < minimumFuel) {
            minimumFuel = fuel;
        }
    }

    int result = minimumFuel;

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day7_run() {
    day7_run_p1();
    day7_run_p2();
}
