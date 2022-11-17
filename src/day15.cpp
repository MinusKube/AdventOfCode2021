#include <iostream>
#include <fstream>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "day15.h"

struct point {
    int x;
    int y;

    friend bool operator==(const point& lhs, const point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
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

static void day15_run_p1() {
    std::ifstream file("../data/day15.txt");

    std::vector<std::vector<int>> grid;

    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            break;
        }

        std::vector<int> line_vec;
        for (char c : line) {
            line_vec.push_back(c - '0');
        }

        grid.push_back(line_vec);
    }

    int rows = grid.size();
    int columns = grid[0].size();

    // Dijkstra algorithm
    std::vector<point> Q;
    std::unordered_map<point, int> dist;
    std::unordered_map<point, point> prev;

    point source{ 0, 0 };
    point target{ columns - 1, rows - 1 };

    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            point p{ x, y };
            point invalidPoint{ -1, -1 };

            dist.insert({ p, 9999999 });
            prev.insert({ p, invalidPoint });
            Q.push_back(p);
        }
    }

    dist[source] = 0;

    while (Q.size() > 0) {
        int minDist = 9999999;
        point minPoint;

        for (point point : Q) {
            if (dist[point] <= minDist) {
                minDist = dist[point];
                minPoint = point;
            }
        }

        Q.erase(std::find(Q.begin(), Q.end(), minPoint));

        point neighbors[4];
        neighbors[0] = point{ minPoint.x - 1, minPoint.y };
        neighbors[1] = point{ minPoint.x + 1, minPoint.y };
        neighbors[2] = point{ minPoint.x, minPoint.y - 1 };
        neighbors[3] = point{ minPoint.x, minPoint.y + 1 };

        for (point neighbor : neighbors) {
            if (std::find(Q.begin(), Q.end(), neighbor) == Q.end()) {
                continue;
            }

            int alt = dist[minPoint] + grid[neighbor.y][neighbor.x];
            if (alt < dist[neighbor]) {
                dist[neighbor] = alt;
                prev[neighbor] = minPoint;
            }
        }
    }

    std::vector<point> S;
    point u = target;
    if (prev[u].x != -1) {
        while (true) {
            S.insert(S.begin(), u);

            if (prev[u].x == -1) {
                break;
            }

            u = prev[u];
        }
    }

    int result = 0;
    for (point a : S) {
        if (a.x == 0 && a.y == 0) {
            continue;
        }

        result += grid[a.y][a.x];
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static int manhattan(point a, point b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

static void day15_run_p2() {
    std::ifstream file("../data/day15.txt");

    std::vector<std::vector<int>> grid;

    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            break;
        }

        std::vector<int> line_vec;
        for (int i = 0; i < 5; i++) {
            for (char c : line) {
                int value = c - '0' + i;
                if (value > 9) {
                    value -= 9;
                }
                line_vec.push_back(value);
            }
        }

        grid.push_back(line_vec);
    }

    int base_rows = grid.size();
    for (int i = 1; i <= 4; i++) {
        for (int row = 0; row < base_rows; row++) {
            std::vector<int> line = grid[row];
            for (int column = 0; column < line.size(); column++) {
                line[column] += i;

                if (line[column] > 9) {
                    line[column] -= 9;
                }
            }

            grid.push_back(line);
        }
    }

    int rows = grid.size();
    int columns = grid[0].size();

    point start{ 0, 0 };
    point target{ columns - 1, rows - 1 };

    // A* Algorithm
    std::unordered_set<point> openSet;
    openSet.insert(start);

    std::unordered_map<point, point> cameFrom;

    std::unordered_map<point, int> gScore;
    std::unordered_map<point, int> fScore;

    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            point p{ x, y };
            point invalidPoint{ -1, -1 };

            gScore.insert({ p, std::numeric_limits<int>::max() });
            fScore.insert({ p, std::numeric_limits<int>::max() });

            cameFrom.insert({ p, invalidPoint });
        }
    }

    gScore[start] = 0;
    fScore[start] = manhattan(start, target);

    point lastPoint;

    while (openSet.size() > 0) {
        int minScore = std::numeric_limits<int>::max();
        point current;

        for (point point : openSet) {
            if (fScore[point] <= minScore) {
                minScore = fScore[point];
                current = point;
            }
        }

        if (current == target) {
            lastPoint = current;
            break;
        }

        openSet.erase(current);

        point neighbors[4];
        neighbors[0] = point{ current.x - 1, current.y };
        neighbors[1] = point{ current.x + 1, current.y };
        neighbors[2] = point{ current.x, current.y - 1 };
        neighbors[3] = point{ current.x, current.y + 1 };

        for (point neighbor : neighbors) {
            if (cameFrom.find(neighbor) == cameFrom.end()) {
                continue;
            }

            int tentative_gScore = gScore[current] + grid[neighbor.y][neighbor.x];

            if (tentative_gScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = tentative_gScore + manhattan(neighbor, target);

                if (openSet.find(neighbor) == openSet.end()) {
                    openSet.insert(neighbor);
                }
            }
        }
    }

    std::vector<point> path;
    path.push_back(lastPoint);

    while (true) {
        lastPoint = cameFrom[lastPoint];

        if (lastPoint.x == -1) {
            break;
        }

        path.insert(path.begin(), lastPoint);
    }

    int result = 0;
    for (point a : path) {
        if (a.x == 0 && a.y == 0) {
            continue;
        }

        result += grid[a.y][a.x];
    }

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day15_run() {
    day15_run_p1();
    day15_run_p2();
}
