#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

#include "day5.h"

struct position {
    int x;
    int y;

    position(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const position& p) const {
        return x == p.x && y == p.y;
    }
};

namespace std {
    template<>
    struct hash<position> {
        size_t operator()(const position& p) const {
            using std::size_t;
            using std::hash;
            using std::string;

            return ((hash<int>()(p.x) ^ (hash<int>()(p.y) << 1)) >> 1);
        }
    };
}

static void day5_run_p1() {
    std::ifstream file("../data/day5.txt");

    std::unordered_map<position, int> points;

    while (file.good()) {
        int x1, x2;
        int y1, y2;

        char ignoredChar;
        std::string ignoredText;

        file >> x1 >> ignoredChar >> y1 >> ignoredText;
        file >> x2 >> ignoredChar >> y2;

        if (!file.good()) {
            break;
        }

        if (x1 == x2) {
            int min = std::min(y1, y2);
            int max = std::max(y1, y2);

            for (int y = min; y <= max; y++) {
                position pos{x1, y};

                if (points.find(pos) != points.end()) {
                    points[pos]++;
                }
                else {
                    points.insert(std::make_pair(pos, 1));
                }
            }
        }
        else if (y1 == y2) {
            int min = std::min(x1, x2);
            int max = std::max(x1, x2);

            for (int x = min; x <= max; x++) {
                position pos{x, y1};

                if (points.find(pos) != points.end()) {
                    points[pos]++;
                }
                else {
                    points.insert(std::make_pair(pos, 1));
                }
            }
        }
    }

    int result = 0;

    for (std::pair<position, int> pair : points) {
        if (pair.second >= 2) {
            result++;
        }
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day5_run_p2() {
    std::ifstream file("../data/day5.txt");

    std::unordered_map<position, int> points;

    while (file.good()) {
        int x1, x2;
        int y1, y2;

        char ignoredChar;
        std::string ignoredText;

        file >> x1 >> ignoredChar >> y1 >> ignoredText;
        file >> x2 >> ignoredChar >> y2;

        if (!file.good()) {
            break;
        }

        if (x1 == x2) {
            int min = std::min(y1, y2);
            int max = std::max(y1, y2);

            for (int y = min; y <= max; y++) {
                position pos{x1, y};

                if (points.find(pos) != points.end()) {
                    points[pos]++;
                }
                else {
                    points.insert(std::make_pair(pos, 1));
                }
            }
        }
        else if (y1 == y2) {
            int min = std::min(x1, x2);
            int max = std::max(x1, x2);

            for (int x = min; x <= max; x++) {
                position pos{x, y1};

                if (points.find(pos) != points.end()) {
                    points[pos]++;
                }
                else {
                    points.insert(std::make_pair(pos, 1));
                }
            }
        }
        else {
            int minX = std::min(x1, x2);
            int maxX = std::max(x1, x2);

            int yDiff, y;

            if (minX == x1) {
                yDiff = y2 - y1;
                y = y1;
            }
            else {
                yDiff = y1 - y2;
                y = y2;
            }

            for (int x = minX; x <= maxX; x++) {
                position pos{x, y};

                if (points.find(pos) != points.end()) {
                    points[pos]++;
                }
                else {
                    points.insert(std::make_pair(pos, 1));
                }

                y += yDiff > 0 ? 1 : -1;
            }
        }
    }

    int result = 0;

    for (std::pair<position, int> pair : points) {
        if (pair.second >= 2) {
            result++;
        }
    }

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day5_run() {
    day5_run_p1();
    day5_run_p2();
}
