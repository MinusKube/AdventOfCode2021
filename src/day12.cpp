#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "day12.h"

static bool is_big_cave(std::string cave) {
    return cave[0] >= 'A' && cave[0] <= 'Z';
}

class path {
public:
    std::vector<std::string> caves;
    bool ended = false;
    int small_cave_visited_twice = false;

    std::vector<path> continue_path(std::unordered_map<std::string, std::vector<std::string>> connections) {
        std::string last_cave = caves[caves.size() - 1];
        std::vector<path> next_paths;

        for (auto next_cave : connections[last_cave]) {
            if (!is_big_cave(next_cave) && std::find(caves.begin(), caves.end(), next_cave) != caves.end()) {
                continue;
            }

            path new_path;
            new_path.caves = caves;
            new_path.caves.push_back(next_cave);

            if (next_cave == "end") {
                new_path.ended = true;
            }

            next_paths.push_back(new_path);
        }

        return next_paths;
    }

    std::vector<path> continue_path_2(std::unordered_map<std::string, std::vector<std::string>> connections) {
        std::string last_cave = caves[caves.size() - 1];
        std::vector<path> next_paths;

        for (auto next_cave : connections[last_cave]) {
            auto will_be_visited_twice = false;
            if (!is_big_cave(next_cave) && std::find(caves.begin(), caves.end(), next_cave) != caves.end()) {
                if (small_cave_visited_twice || next_cave == "start" || next_cave == "end") {
                    continue;
                }
                else {
                    will_be_visited_twice = true;
                }
            }

            path new_path;
            new_path.caves = caves;
            new_path.caves.push_back(next_cave);
            new_path.small_cave_visited_twice = small_cave_visited_twice || will_be_visited_twice;

            if (next_cave == "end") {
                new_path.ended = true;
            }

            next_paths.push_back(new_path);
        }

        return next_paths;
    }
};

static void day12_run_p1() {
    std::ifstream file("../data/day12.txt");

    std::unordered_map<std::string, std::vector<std::string>> connections;
    std::string line;
    while (file >> line) {
        size_t separator = line.find('-');

        std::string a = line.substr(0, separator);
        std::string b = line.substr(separator + 1, line.size() - separator - 1);

        if (connections.find(a) == connections.end()) {
            std::vector<std::string> vec;
            vec.push_back(b);

            connections.emplace(a, vec);
        }
        else {
            connections[a].push_back(b);
        }

        if (connections.find(b) == connections.end()) {
            std::vector<std::string> vec;
            vec.push_back(a);

            connections.emplace(b, vec);
        }
        else {
            connections[b].push_back(a);
        }
    }

    path startPath;
    startPath.caves.push_back("start");

    std::vector<path> last_paths;
    last_paths.push_back(startPath);

    std::vector<path> complete_paths;
    while (last_paths.size() > 0) {
        std::vector<path> new_paths;
        for (auto path : last_paths) {
            for (auto next_path : path.continue_path(connections)) {
                if (next_path.ended) {
                    complete_paths.push_back(next_path);
                }
                else {
                    new_paths.push_back(next_path);
                }
            }
        }

        last_paths = new_paths;
    }

    int result = complete_paths.size();
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day12_run_p2() {
    std::ifstream file("../data/day12.txt");

    std::unordered_map<std::string, std::vector<std::string>> connections;
    std::string line;
    while (file >> line) {
        size_t separator = line.find('-');

        std::string a = line.substr(0, separator);
        std::string b = line.substr(separator + 1, line.size() - separator - 1);

        if (connections.find(a) == connections.end()) {
            std::vector<std::string> vec;
            vec.push_back(b);

            connections.emplace(a, vec);
        }
        else {
            connections[a].push_back(b);
        }

        if (connections.find(b) == connections.end()) {
            std::vector<std::string> vec;
            vec.push_back(a);

            connections.emplace(b, vec);
        }
        else {
            connections[b].push_back(a);
        }
    }

    path startPath;
    startPath.caves.push_back("start");

    std::vector<path> last_paths;
    last_paths.push_back(startPath);

    std::vector<path> complete_paths;
    while (last_paths.size() > 0) {
        std::vector<path> new_paths;
        for (auto path : last_paths) {
            for (auto next_path : path.continue_path_2(connections)) {
                if (next_path.ended) {
                    complete_paths.push_back(next_path);
                }
                else {
                    new_paths.push_back(next_path);
                }
            }
        }

        last_paths = new_paths;
    }

    int result = complete_paths.size();
    std::cout << "Part 2 Result: " << result << std::endl;
}

void day12_run() {
    day12_run_p1();
    day12_run_p2();
}
