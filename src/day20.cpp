#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "day20.h"

static int compute_index(bool** grid, int rows, int columns, int cur_row, int cur_column) {
    int index = 0;

    for (int row = cur_row - 1; row <= cur_row + 1; row++) {
        for (int column = cur_column - 1; column <= cur_column +1; column++) {
            int value;

            if (row < 0 || row >= rows ||
                column < 0 || column >= columns) {

                value = 0;
            }
            else {
                value = grid[row][column];
            }

            index = (index << 1) | value;
        }
    }

    return index;
}

constexpr int PADDING_BIG = 8;
constexpr int PADDING_SMALL = 4;

static bool** apply_borders(bool** grid, int rows, int columns) {
    bool** new_grid = new bool*[rows + PADDING_BIG];
    for (int row = 0; row < rows + PADDING_BIG; row++) {
        new_grid[row] = new bool[columns + PADDING_BIG];

        for (int column = 0; column < columns + PADDING_BIG; column++) {
            int oldRow = row - PADDING_SMALL;
            int oldColumn = column - PADDING_SMALL;

            bool value;

            if (oldRow < 0 || oldRow >= rows ||
                oldColumn < 0 || oldColumn >= columns) {

                value = 0;
            }
            else {
                value = grid[oldRow][oldColumn];
            }

            new_grid[row][column] = value;
        }
    }

    return new_grid;
}

static bool** apply_step(bool* algorithm, bool** grid, int rows, int columns) {
    bool** new_grid = new bool*[rows + PADDING_BIG];
    for (int row = 0; row < rows + PADDING_BIG; row++) {
        new_grid[row] = new bool[columns + PADDING_BIG];

        for (int column = 0; column < columns + PADDING_BIG; column++) {
            int checkRow = row - PADDING_SMALL;
            int checkColumn = column - PADDING_SMALL;

            checkRow = std::max(checkRow, 1);
            checkRow = std::min(checkRow, rows - 2);

            checkColumn = std::max(checkColumn, 1);
            checkColumn = std::min(checkColumn, columns - 2);

            int index = compute_index(grid, rows, columns, checkRow, checkColumn);
            bool value = algorithm[index];

            new_grid[row][column] = value;
        }
    }

    return new_grid;
}

static int run_steps_and_count_light_cells(bool* algorithm, std::vector<std::string> lines, int step_count) {
    int rows = lines.size();
    int columns = lines[0].size();

    bool** grid = new bool*[rows];
    for (int row = 0; row < rows; row++) {
        grid[row] = new bool[columns];

        for (int column = 0; column < columns; column++) {
            grid[row][column] = (lines[row][column] == '#');
        }
    }

    grid = apply_borders(grid, rows, columns);
    rows += PADDING_BIG;
    columns += PADDING_BIG;

    for (int i = 0; i < step_count; i++) {
        grid = apply_step(algorithm, grid, rows, columns);
        rows += PADDING_BIG;
        columns += PADDING_BIG;
    }

    int light_count = 0;
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (grid[row][column]) {
                light_count += 1;
            }
        }
    }

    return light_count;
}

static void day20_run_p1() {
    std::ifstream file("../data/day20.txt");

    std::string algorithm_str;
    file >> algorithm_str;

    bool* algorithm = new bool[algorithm_str.size()];
    for (int i = 0; i < algorithm_str.size(); i++) {
        algorithm[i] = (algorithm_str[i] == '#');
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            continue;
        }

        lines.push_back(line);
    }

    int light_count = run_steps_and_count_light_cells(algorithm, lines, 2);
    std::cout << "Part 1 Result: " << light_count << std::endl;
}

static void day20_run_p2() {
    std::ifstream file("../data/day20.txt");

    std::string algorithm_str;
    file >> algorithm_str;

    bool* algorithm = new bool[algorithm_str.size()];
    for (int i = 0; i < algorithm_str.size(); i++) {
        algorithm[i] = (algorithm_str[i] == '#');
    }

    std::vector<std::string> lines;

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            continue;
        }

        lines.push_back(line);
    }

    int light_count = run_steps_and_count_light_cells(algorithm, lines, 50);
    std::cout << "Part 2 Result: " << light_count << std::endl;
}

void day20_run() {
    day20_run_p1();
    day20_run_p2();
}
