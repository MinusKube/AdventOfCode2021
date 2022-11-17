#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "day9.h"

static void day9_run_p1() {
    std::ifstream file("../data/day9.txt");

    std::vector<std::string> lines;
    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            break;
        }

        lines.push_back(line);
    }

    int width = lines[0].size();
    int height = lines.size();

    int* values = new int[width * height];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            values[y * width + x] = lines[y][x] - '0';
        }
    }

    int result = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int value = values[y * width + x];

            int left = x > 0 ? values[y * width + (x - 1)] : -1;
            int right = x < width - 1 ? values[y * width + (x + 1)] : -1;
            int up = y > 0 ? values[(y - 1) * width + x] : -1;
            int down = y < height - 1 ? values[(y + 1) * width + x] : -1;

            if ((left == -1 || left > value) &&
                (right == -1 || right > value) &&
                (up == -1 || up > value) &&
                (down == -1 || down > value)) {

                result += (value + 1);
            }
        }
    }

    delete[] values;

    std::cout << "Part 1 Result: " << result << std::endl;
}

static int floodFill(int* values, int width, int height, std::vector<int>& handled, int x, int y) {
    if (std::find(handled.begin(), handled.end(), y * width + x) != handled.end()) {
        return 0;
    }

    int count = 1;
    int value = values[y * width + x];

    if (value >= 9) {
        return 0;
    }

    handled.push_back(y * width + x);

    if (x > 0 && values[y * width + (x - 1)] >= value) count += floodFill(values, width, height, handled, x - 1, y);
    if (x < width - 1 && values[y * width + (x + 1)] >= value) count += floodFill(values, width, height, handled, x + 1, y);
    if (y > 0 && values[(y - 1) * width + x] >= value) count += floodFill(values, width, height, handled, x, y - 1);
    if (y < height - 1 && values[(y + 1) * width + x] >= value) count += floodFill(values, width, height, handled, x, y + 1);

    return count;
}

static void day9_run_p2() {
    std::ifstream file("../data/day9.txt");

    std::vector<std::string> lines;
    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            break;
        }

        lines.push_back(line);
    }

    int width = lines[0].size();
    int height = lines.size();

    int* values = new int[width * height];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            values[y * width + x] = lines[y][x] - '0';
        }
    }

    std::vector<int> basinLowPoints;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int value = values[y * width + x];

            int left = x > 0 ? values[y * width + (x - 1)] : -1;
            int right = x < width - 1 ? values[y * width + (x + 1)] : -1;
            int up = y > 0 ? values[(y - 1) * width + x] : -1;
            int down = y < height - 1 ? values[(y + 1) * width + x] : -1;

            if ((left == -1 || left > value) &&
                (right == -1 || right > value) &&
                (up == -1 || up > value) &&
                (down == -1 || down > value)) {

                basinLowPoints.push_back(y * width + x);
            }
        }
    }

    std::vector<int> basinSizes;

    for (int lowPoint : basinLowPoints) {
        int x = lowPoint % width;
        int y = lowPoint / width;

        std::vector<int> handledPoints;
        int basin = floodFill(values, width, height, handledPoints, x, y);
        basinSizes.push_back(basin);

        /*for (int a = 0; a < height; a++) {
            for (int b = 0; b < width; b++) {
                if (std::find(handledPoints.begin(), handledPoints.end(), a * width + b) != handledPoints.end()) {
                    std::cout << "#";
                }
                else {
                    std::cout << ".";
                }
            }

            std::cout << std::endl;
            }*/
    }

    std::sort(basinSizes.begin(), basinSizes.end());

    int result = 1;
    for (int i = 0; i < 3; i++) {
        result *= basinSizes[basinSizes.size() - i - 1];
    }

    delete[] values;

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day9_run() {
    day9_run_p1();
    day9_run_p2();
}
