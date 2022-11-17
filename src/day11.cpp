#include <iostream>
#include <fstream>

#include "day11.h"

constexpr int GRID_SIZE = 10;

static void step_flash(int** grid, int& flashCount, int x, int y) {
    grid[x][y] = 0;
    flashCount += 1;

    for (int newX = x - 1; newX <= x + 1; newX++) {
        for (int newY = y - 1; newY <= y + 1; newY++) {
            if (newX < 0 || newX >= GRID_SIZE ||
                newY < 0 || newY >= GRID_SIZE) {

                continue;
            }

            if (grid[newX][newY] != 0) { // 0 = Already flashed
                grid[newX][newY] += 1;
            }

            if (grid[newX][newY] >= 10) { // 10+ = Flash
                step_flash(grid, flashCount, newX, newY);
            }
        }
    }
}

static void step(int** grid, int& flashCount) {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            grid[x][y] += 1;
        }
    }

    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if (grid[x][y] < 10) {
                continue;
            }

            step_flash(grid, flashCount, x, y);
        }
    }
}

static void day11_run_p1() {
    std::ifstream file("../data/day11.txt");

    int** grid = new int*[GRID_SIZE];
    for (int x = 0; x < GRID_SIZE; x++) {
        grid[x] = new int[GRID_SIZE];
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            char c;
            file >> c;

            grid[x][y] = c - '0';
        }
    }

    int flashCount = 0;
    for (int i = 0; i < 100; i++) {
        step(grid, flashCount);
    }

    std::cout << "Part 1 Result: " << flashCount << std::endl;
}

static void day11_run_p2() {
    std::ifstream file("../data/day11.txt");

    int** grid = new int*[GRID_SIZE];
    for (int x = 0; x < GRID_SIZE; x++) {
        grid[x] = new int[GRID_SIZE];
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            char c;
            file >> c;

            grid[x][y] = c - '0';
        }
    }

    int lastFlashCount = 0;
    int flashCount = 0;
    int stepCount = 0;
    while (flashCount - lastFlashCount != 100) {
        lastFlashCount = flashCount;
        step(grid, flashCount);

        stepCount += 1;
    }

    std::cout << "Part 2 Result: " << stepCount << std::endl;
}

void day11_run() {
    day11_run_p1();
    day11_run_p2();
}
