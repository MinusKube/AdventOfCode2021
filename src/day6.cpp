#include <iostream>
#include <fstream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "day6.h"

static void day6_run_p1() {
    std::ifstream file("../data/day6.txt");

    std::vector<int> fishes;
    while (file.good()) {
        int daysLeft;
        char ignored;
        file >> daysLeft >> ignored;

        fishes.push_back(daysLeft);
    }

    for (int i = 0; i < 80; i++) {
        int fishCount = fishes.size();
        for (int j = 0; j < fishCount; j++) {
            fishes[j]--;

            if (fishes[j] < 0) {
                fishes[j] = 6;
                fishes.push_back(8);
            }
        }
    }

    int result = fishes.size();
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day6_run_p2() {
    std::ifstream file("../data/day6.txt");

    std::unordered_map<int, size_t> fishes;
    while (file.good()) {
        int daysLeft;
        char ignored;
        file >> daysLeft >> ignored;

        if (fishes.find(daysLeft) != fishes.end()) {
            fishes[daysLeft]++;
        }
        else {
            fishes.insert(std::make_pair(daysLeft, 1));
        }
    }

    for (int daysLeft = 0; daysLeft <= 8; daysLeft++) {
        if (fishes.find(daysLeft) == fishes.end()) {
            fishes.insert(std::make_pair(daysLeft, 0));
        }
    }

    for (int i = 0; i < 256; i++) {
        std::unordered_map<int, size_t> newFishes;
        for (int daysLeft = 8; daysLeft >= 0; daysLeft--) {
            int newDaysLeft = daysLeft - 1;
            size_t fishCount = fishes[daysLeft];

            if (newDaysLeft < 0) {
                newFishes[8] += fishCount;
                newFishes[6] += fishCount;
            }
            else {
                newFishes.insert(std::make_pair(newDaysLeft, fishCount));
            }
        }

        fishes = newFishes;
    }

    size_t result = 0;
    for (int daysLeft = 0; daysLeft <= 8; daysLeft++) {
        result += fishes[daysLeft];
    }

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day6_run() {
    day6_run_p1();
    day6_run_p2();
}
