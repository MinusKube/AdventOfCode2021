#include <iostream>
#include <fstream>
#include <limits>
#include <unordered_map>
#include <vector>

#include "day14.h"

struct insertion_rule {
    std::string pattern;
    std::string insertion;
};

static std::string apply_rules(std::string polymer, std::vector<insertion_rule> rules) {
    std::string new_polymer;

    for (size_t i = 1; i < polymer.size(); i++) {
        char c1 = polymer[i - 1];
        char c2 = polymer[i];

        std::string insertion;

        for (insertion_rule rule : rules) {
            if (rule.pattern[0] == c1 && rule.pattern[1] == c2) {
                insertion = rule.insertion;
                break;
            }
        }

        new_polymer += c1;
        new_polymer += insertion;
    }

    new_polymer += polymer[polymer.size() - 1];
    return new_polymer;
}

static std::unordered_map<int, long long> apply_rules_fast(std::unordered_map<int, long long> polymer, std::unordered_map<int, char> rules) {
    std::unordered_map<int, long long> new_polymer;

    for (std::pair<int, long long> pair : polymer) {
        char a = pair.first;
        char b = pair.first >> 8;

        int new_pair1 = a | (rules[pair.first] << 8);
        int new_pair2 = rules[pair.first] | (b << 8);

        if (new_polymer.find(new_pair1) != new_polymer.end()) {
            new_polymer[new_pair1] += pair.second;
        }
        else {
            new_polymer.insert({ new_pair1, pair.second });
        }

        if (new_polymer.find(new_pair2) != new_polymer.end()) {
            new_polymer[new_pair2] += pair.second;
        }
        else {
            new_polymer.insert({ new_pair2, pair.second });
        }
    }

    return new_polymer;
}

static void day14_run_p1() {
    std::ifstream file("../data/day14.txt");

    std::vector<insertion_rule> rules;

    std::string polymer;
    file >> polymer;

    while (file.good()) {
        std::string pattern;
        std::string ignored;
        std::string insertion;

        file >> pattern;
        file >> ignored;
        file >> insertion;

        if (!file.good()) {
            continue;
        }

        insertion_rule rule{ pattern, insertion };
        rules.push_back(rule);
    }

    for (int i = 0; i < 10; i++) {
        polymer = apply_rules(polymer, rules);
    }

    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();
    for (char c = 'A'; c <= 'Z'; c++) {
        int count = 0;
        for (char polymer_c : polymer) {
            if (polymer_c == c) {
                count += 1;
            }
        }

        if (count == 0) {
            continue;
        }

        min = std::min(min, count);
        max = std::max(max, count);
    }

    int result = max - min;
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day14_run_p2() {
    std::ifstream file("../data/day14.txt");

    std::unordered_map<int, char> rules;

    std::string polymer_str;
    file >> polymer_str;

    std::unordered_map<int, long long> polymer;
    for (size_t i = 1; i < polymer_str.size(); i++) {
        int pair = polymer_str[i - 1] | (polymer_str[i] << 8);

        if (polymer.find(pair) != polymer.end()) {
            polymer[pair] += 1;
        }
        else {
            polymer.insert({ pair, 1 });
        }
    }

    while (file.good()) {
        std::string pattern;
        std::string ignored;
        std::string insertion;

        file >> pattern;
        file >> ignored;
        file >> insertion;

        if (!file.good()) {
            continue;
        }

        rules.insert({ pattern[0] | (pattern[1] << 8), insertion[0] });
    }

    for (int i = 0; i < 40; i++) {
        polymer = apply_rules_fast(polymer, rules);
    }

    std::unordered_map<char, long long> chars;

    for (std::pair<int, long long> pair : polymer) {
        char a = pair.first;
        char b = pair.first >> 8;

        if (chars.find(a) != chars.end()) {
            chars[a] += pair.second;
        }
        else {
            chars.insert({ a, pair.second });
        }

        if (chars.find(b) != chars.end()) {
            chars[b] += pair.second;
        }
        else {
            chars.insert({ b, pair.second });
        }
    }

    chars[polymer_str[0]] += 1;
    chars[polymer_str[polymer_str.size() - 1]] += 1;

    long long min = std::numeric_limits<long long>::max();
    long long max = std::numeric_limits<long long>::min();
    for (char c = 'A'; c <= 'Z'; c++) {
        long long count = chars[c] / 2;

        if (count == 0) {
            continue;
        }

        min = std::min(min, count);
        max = std::max(max, count);
    }

    long long result = max - min;
    std::cout << "Part 2 Result: " << result << std::endl;
}

void day14_run() {
    day14_run_p1();
    day14_run_p2();
}
