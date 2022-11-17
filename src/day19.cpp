#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "day19.h"

constexpr int NEEDED_MATCHING_POINTS = 12;
constexpr int ROTATION_COUNT = 24;

struct point3 {
    int x;
    int y;
    int z;

    point3 operator-(const point3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    point3 operator+(const point3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    bool operator==(const point3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const point3& other) const {
        return x != other.x || y != other.y || z != other.z;
    }
};

namespace std {
    template<>
    struct hash<point3> {
        size_t operator()(const point3& p) const {
            using std::size_t;
            using std::hash;
            using std::string;

            size_t result = hash<int>()(p.x);
            result ^= (hash<int>()(p.y) << 1);
            result >>= 1;
            result ^= (hash<int>()(p.z) << 1);
            result >>= 1;

            return result;
        }
    };
}

struct scanner {
    std::vector<point3> beacons[ROTATION_COUNT];
};

struct find_result {
    bool found;
    point3 offset;
    std::vector<point3> matching_points;
};

struct dependency {
    int index;
    int matching_rotation;
    find_result result;
};

static point3 r_z(const point3& p) {
    return { -p.y, p.x, p.z };
}

static point3 r_y(const point3& p) {
    return { p.z, p.y, -p.x };
}

static point3 r_x(const point3& p) {
    return { p.x, -p.z, p.y };
}

static point3 rotate_point(const point3& p, int rotation) {
    switch (rotation) {
    case 0: return p;
    case 1: return r_x(p);
    case 2: return r_y(p);
    case 3: return r_x(r_x(p));
    case 4: return r_y(r_x(p));
    case 5: return r_x(r_y(p));
    case 6: return r_y(r_y(p));
    case 7: return r_x(r_x(r_x(p)));
    case 8: return r_y(r_x(r_x(p)));
    case 9: return r_x(r_y(r_x(p)));
    case 10: return r_y(r_y(r_x(p)));
    case 11: return r_x(r_x(r_y(p)));
    case 12: return r_x(r_y(r_y(p)));
    case 13: return r_y(r_y(r_y(p)));
    case 14: return r_y(r_x(r_x(r_x(p))));
    case 15: return r_x(r_y(r_x(r_x(p))));
    case 16: return r_y(r_y(r_x(r_x(p))));
    case 17: return r_x(r_x(r_y(r_x(p))));
    case 18: return r_y(r_y(r_y(r_x(p))));
    case 19: return r_x(r_x(r_x(r_y(p))));
    case 20: return r_x(r_y(r_y(r_y(p))));
    case 21: return r_x(r_y(r_x(r_x(r_x(p)))));
    case 22: return r_x(r_x(r_x(r_y(r_x(p)))));
    case 23: return r_x(r_y(r_y(r_y(r_x(p)))));
    }

    throw std::exception("Invalid rotation index");
}

static std::vector<point3> rotate_beacons(const std::vector<point3>& beacons, int rotation) {
    std::vector<point3> result;
    for (const point3& p : beacons) {
        result.push_back(rotate_point(p, rotation));
    }

    return result;
}

static scanner compute_scanner_rotations(const std::vector<point3>& beacons) {
    scanner result;
    for (int i = 0; i < ROTATION_COUNT; i++) {
        result.beacons[i] = rotate_beacons(beacons, i);
    }

    return result;
}

static find_result find_beacons_offset(const std::vector<point3>& a, const std::vector<point3>& b) {
    for (point3 a_point : a) {
        for (point3 b_point : b) {
            point3 offset = a_point - b_point;
            std::vector<point3> matching_points;

            for (point3 b_other_point : b) {
                if (std::find(a.begin(), a.end(), b_other_point + offset) == a.end()) {
                    continue;
                }

                matching_points.push_back(b_other_point + offset);
            }

            if (matching_points.size() >= NEEDED_MATCHING_POINTS) {
                return { true, offset, matching_points };
            }
        }
    }

    return { false, { 0, 0, 0 }, {} };
}

void day19_run() {
    std::ifstream file("../data/day19.txt");

    std::vector<scanner> scanners;

    std::string line;
    std::getline(file, line); // Ignore scanner header

    std::vector<point3> current_beacons;
    while (std::getline(file, line)) {
        if (line.size() == 0) {
            scanner s = compute_scanner_rotations(current_beacons);
            scanners.push_back(std::move(s));

            current_beacons.clear();
            std::getline(file, line); // Ignore scanner header
            continue;
        }

        std::stringstream stream(line);
        point3 p;
        char ignored;

        stream >> p.x >> ignored >> p.y >> ignored >> p.z;
        current_beacons.push_back(p);
    }

    {
        scanner s = compute_scanner_rotations(current_beacons);
        scanners.push_back(std::move(s));
    }

    std::unordered_map<int, int> matching_rotations;
    matching_rotations.insert({ 0, 0 });

    std::unordered_map<int, std::vector<dependency>> dependencies;

    for (int a = 0; a < scanners.size(); a++) {
        scanner& scanner_a = scanners[a];
        dependencies.insert({ a, {} });

        for (int b = 0; b < scanners.size(); b++) {
            if (a == b) {
                continue;
            }

            scanner& scanner_b = scanners[b];

            for (int rotation = 0; rotation < ROTATION_COUNT; rotation++) {
                find_result result = find_beacons_offset(scanner_a.beacons[0], scanner_b.beacons[rotation]);

                if (!result.found) {
                    continue;
                }

                dependency d;
                d.index = b;
                d.matching_rotation = rotation;
                d.result = result;

                matching_rotations.insert({ b, rotation });
                dependencies[a].push_back(d);
                std::cout << b << " depends on " << a << std::endl;
                break;
            }
        }
    }

    std::unordered_map<int, point3> offsets_from_0;
    offsets_from_0.insert({ 0, { 0, 0, 0 } });

    std::unordered_set<point3> beacon_list;
    for (auto beacon : scanners[0].beacons[0]) {
        beacon_list.insert(beacon);
    }

    std::vector<int> next_dependencies;
    next_dependencies.push_back(0);

    std::cout << "Apply 0" << std::endl;

    while (next_dependencies.size() > 0) {
        int a = next_dependencies[next_dependencies.size() - 1];
        next_dependencies.pop_back();

        scanner& scanner_a = scanners[a];

        for (dependency d : dependencies[a]) {
            int b = d.index;
            scanner& scanner_b = scanners[b];

            if (offsets_from_0.find(b) != offsets_from_0.end()) {
                // Already computed
                continue;
            }

            for (int rotation = 0; rotation < ROTATION_COUNT; rotation++) {
                find_result new_result = find_beacons_offset(scanner_a.beacons[matching_rotations[a]], scanner_b.beacons[rotation]);

                if (!new_result.found) {
                    continue;
                }

                point3 total_offset = offsets_from_0[a] + new_result.offset;
                offsets_from_0.insert({ b, total_offset });

                for (point3 p : scanner_b.beacons[rotation]) {
                    beacon_list.insert(p + total_offset);
                }

                matching_rotations[b] = rotation;

                std::cout << "Apply " << b << " (rot " << rotation << ") "
                          << "(depends on " << a << " with rotation " << matching_rotations[a] << ")" << std::endl;

                next_dependencies.push_back(b);
                break;
            }
        }
    }

    long long highest_distance = 0;
    for (int a = 0; a < scanners.size(); a++) {
        point3 a_pos = offsets_from_0[a];

        for (int b = a + 1; b < scanners.size(); b++) {
            point3 b_pos = offsets_from_0[b];

            long long dist = std::abs(a_pos.x - b_pos.x) +
                std::abs(a_pos.y - b_pos.y) +
                std::abs(a_pos.z - b_pos.z);

            highest_distance = std::max(highest_distance, dist);
        }
    }

    int p1_result = beacon_list.size();
    std::cout << "Part 1 Result: " << p1_result << std::endl;

    long long p2_result = highest_distance;
    std::cout << "Part 2 Result: " << p2_result << std::endl;
}
