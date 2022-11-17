#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>

#include "day13.h"

struct point {
    int x;
    int y;

    int& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;

        throw std::exception("Invalid index for operator [] on point class");
    }

    friend bool operator==(const point& lhs, const point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct fold_instruction {
    bool x_axis;
    int value;
};

namespace std {
    template<>
    struct hash<point> {
        size_t operator()(const point& p) const {
            using std::size_t;
            using std::hash;
            using std::string;

            return ((hash<int>()(p.x) ^ (hash<int>()(p.y) << 1)) >> 1);
        }
    };
}

static void day13_run_p1() {
    std::ifstream file("../data/day13.txt");

    std::vector<point> points;
    std::vector<fold_instruction> fold_instructions;

    std::string line;
    bool foldLine = false;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            foldLine = true;
            continue;
        }

        if (!foldLine) {
            size_t delimiter = line.find(',');
            std::string x_str = line.substr(0, delimiter);
            std::string y_str = line.substr(delimiter + 1);

            point point{ std::stoi(x_str), std::stoi(y_str) };
            points.push_back(point);
        }
        else {
            size_t delimiter = line.find('=');
            std::string axis = line.substr(delimiter - 1, 1);
            std::string value_str = line.substr(delimiter + 1);

            fold_instruction instr{ axis == "x", std::stoi(value_str) };
            fold_instructions.push_back(instr);
        }
    }

    std::unordered_set<point> points_distinct;

    fold_instruction instr = fold_instructions[0];
    int axis_index = instr.x_axis ? 0 : 1;
    for (point point : points) {
        if (point[axis_index] > instr.value) {
            int dist_to_fold = point[axis_index] - instr.value;
            point[axis_index] = instr.value - dist_to_fold;
        }

        points_distinct.insert(point);
    }

    int result = points_distinct.size();
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day13_run_p2() {
    std::ifstream file("../data/day13.txt");

    std::vector<point> points;
    std::vector<fold_instruction> fold_instructions;

    std::string line;
    bool foldLine = false;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            foldLine = true;
            continue;
        }

        if (!foldLine) {
            size_t delimiter = line.find(',');
            std::string x_str = line.substr(0, delimiter);
            std::string y_str = line.substr(delimiter + 1);

            point point{ std::stoi(x_str), std::stoi(y_str) };
            points.push_back(point);
        }
        else {
            size_t delimiter = line.find('=');
            std::string axis = line.substr(delimiter - 1, 1);
            std::string value_str = line.substr(delimiter + 1);

            fold_instruction instr{ axis == "x", std::stoi(value_str) };
            fold_instructions.push_back(instr);
        }
    }

    for (fold_instruction instr : fold_instructions) {
        std::unordered_set<point> points_distinct;

        int axis_index = instr.x_axis ? 0 : 1;
        for (point point : points) {
            if (point[axis_index] > instr.value) {
                int dist_to_fold = point[axis_index] - instr.value;
                point[axis_index] = instr.value - dist_to_fold;
            }

            points_distinct.insert(point);
        }

        points.clear();
        for (point point : points_distinct) {
            points.push_back(point);
        }
    }

    int minX = 9999;
    int maxX = 0;
    int minY = 9999;
    int maxY = 0;
    for (point point : points) {
        minX = std::min(point.x, minX);
        maxX = std::max(point.x, maxX);
        minY = std::min(point.y, minY);
        maxY = std::max(point.y, maxY);
    }

    std::cout << "Part 2 Result: " << std::endl;
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (std::find(points.begin(), points.end(), point{ x, y }) != points.end()) {
                std::cout << "#";
            }
            else {
                std::cout << ".";
            }
        }

        std::cout << std::endl;
    }
}

void day13_run() {
    day13_run_p1();
    day13_run_p2();
}
