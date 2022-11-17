#include <iostream>
#include <fstream>
#include <string>

#include "day2.h"

static void day2_run_p1() {
    std::ifstream file("../data/day2.txt");

    int horizontal_pos = 0;
    int depth = 0;

    std::string current_dir;
    int current_dir_amount;

    while (file >> current_dir >> current_dir_amount) {
        if (current_dir == "forward") {
            horizontal_pos += current_dir_amount;
        }
        else if (current_dir == "up") {
            depth -= current_dir_amount;
        }
        else if (current_dir == "down") {
            depth += current_dir_amount;
        }
    }

    int result = horizontal_pos * depth;
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day2_run_p2() {
    std::ifstream file("../data/day2.txt");

    int horizontal_pos = 0;
    int depth = 0;
    int aim = 0;

    std::string current_dir;
    int current_dir_amount;

    while (file >> current_dir >> current_dir_amount) {
        if (current_dir == "forward") {
            horizontal_pos += current_dir_amount;
            depth += aim * current_dir_amount;
        }
        else if (current_dir == "up") {
            aim -= current_dir_amount;
        }
        else if (current_dir == "down") {
            aim += current_dir_amount;
        }
    }

    int result = horizontal_pos * depth;
    std::cout << "Part 2 Result: " << result << std::endl;
}

void day2_run() {
    day2_run_p1();
    day2_run_p2();
}
