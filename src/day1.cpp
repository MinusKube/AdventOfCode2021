#include <iostream>
#include <fstream>

#include "day1.h"

static void day1_run_p1() {
    std::ifstream file("../data/day1.txt");

    int result = 0;

    int last_depth;
    file >> last_depth;

    int depth;
    while (file >> depth) {
        if (depth > last_depth) {
            result += 1;
        }

        last_depth = depth;
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day1_run_p2() {
    std::ifstream file("../data/day1.txt");

    int result = 0;

    int last_depths[3];
    file >> last_depths[0];
    file >> last_depths[1];
    file >> last_depths[2];

    int depth;
    while (file >> depth) {
        int last_sum = last_depths[0] + last_depths[1] + last_depths[2];
        int sum = last_depths[1] + last_depths[2] + depth;

        if (sum > last_sum) {
            result += 1;
        }

        last_depths[0] = last_depths[1];
        last_depths[1] = last_depths[2];
        last_depths[2] = depth;
    }

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day1_run() {
    day1_run_p1();
    day1_run_p2();
}
