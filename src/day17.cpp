#include <iostream>
#include <fstream>
#include <vector>

#include "day17.h"

struct bounds {
    int min_x, max_x;
    int min_y, max_y;
};

struct result {
    bool hit;
    int highest_y;
};

struct velocity {
    int x;
    int y;
};

static result will_hit_bounds(int vel_x, int vel_y, bounds b) {
    int pos_x = 0;
    int pos_y = 0;

    int highest_y = 0;

    while (true) {
        pos_x += vel_x;
        pos_y += vel_y;

        highest_y = std::max(highest_y, pos_y);

        if (pos_x >= b.min_x && pos_x <= b.max_x &&
            pos_y >= b.min_y && pos_y <= b.max_y) {

            return { true, highest_y };
        }

        if (vel_x > 0) vel_x -= 1;
        if (vel_x < 0) vel_x += 1;
        vel_y -= 1;

        if (vel_y < 0 && pos_y < b.min_y) {
            break;
        }
    }

    return { false, highest_y };
}

static void day17_run_p1() {
    std::ifstream file("../data/day17.txt");

    int min_x, max_x;
    int min_y, max_y;

    char ignored[4];

    file.seekg(15);
    file >> min_x;

    file.read(ignored, 2);
    file >> max_x;

    file.read(ignored, 4);
    file >> min_y;

    file.read(ignored, 2);
    file >> max_y;

    bounds b{ min_x, max_x, min_y, max_y };
    result the_result{ false, 0 };

    int y = 1000;
    while (!the_result.hit) {
        for (int x = 0; x < b.max_x; x++) {
            result r = will_hit_bounds(x, y, b);

            if (r.hit) {
                the_result = r;
            }
        }

        y -= 1;
    }

    std::cout << "Part 1 Result: " << the_result.highest_y << std::endl;
}

static void day17_run_p2() {
    std::ifstream file("../data/day17.txt");

    int min_x, max_x;
    int min_y, max_y;

    char ignored[4];

    file.seekg(15);
    file >> min_x;

    file.read(ignored, 2);
    file >> max_x;

    file.read(ignored, 4);
    file >> min_y;

    file.read(ignored, 2);
    file >> max_y;

    bounds b{ min_x, max_x, min_y, max_y };
    std::vector<velocity> velocities;

    for (int y = -1000; y < 1000; y++) {
        for (int x = 0; x < 1000; x++) {
            result r = will_hit_bounds(x, y, b);

            if (r.hit) {
                velocities.push_back({ x, y });
            }
        }
    }

    std::cout << "Part 2 Result: " << velocities.size() << std::endl;
}

void day17_run() {
    day17_run_p1();
    day17_run_p2();
}
