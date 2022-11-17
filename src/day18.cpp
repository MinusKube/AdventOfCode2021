#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "day18.h"

struct pair;

union pair_value {
    pair* p;
    int   i;
};

struct pair {
    bool left_is_pair;
    pair_value left;
    bool right_is_pair;
    pair_value right;

    pair* parent;
};

static pair* parse_pair(std::stringstream& stream, pair* parent = nullptr) {
    pair* p = new pair();
    p->parent = parent;

    stream.get(); // '['

    char left_c;
    stream.get(left_c);

    if (left_c == '[') {
        stream.unget();
        p->left.p = parse_pair(stream, p);
        p->left_is_pair = true;
    }
    else {
        p->left.i = (left_c - '0');
        p->left_is_pair = false;
    }

    stream.get(); // ','

    char right_c;
    stream.get(right_c);

    if (right_c == '[') {
        stream.unget();
        p->right.p = parse_pair(stream, p);
        p->right_is_pair = true;
    }
    else {
        p->right.i = (right_c - '0');
        p->right_is_pair = false;
    }

    stream.get(); // ']'

    return p;
}

static void debug_pair(const pair& p) {
    std::cout << "[";

    if (p.left_is_pair) {
        debug_pair(*p.left.p);
    }
    else {
        std::cout << p.left.i;
    }

    std::cout << ",";

    if (p.right_is_pair) {
        debug_pair(*p.right.p);
    }
    else {
        std::cout << p.right.i;
    }

    std::cout << "]";

    if (!p.parent) {
        std::cout << std::endl;
    }
}

static int* find_left_number(pair* base, pair* current) {
    if (current == nullptr) {
        return nullptr;
    }

    if (current->right_is_pair && current->right.p == base) {
        if (current->left_is_pair) {
            return find_left_number(base, current->left.p);
        }
        else {
            return &current->left.i;
        }
    }

    if (current->left_is_pair && current->left.p == base) {
        return find_left_number(current, current->parent);
    }

    if (!current->right_is_pair) {
        return &current->right.i;
    }
    else {
        return find_left_number(base, current->right.p);
    }
}

static int* find_right_number(pair* base, pair* current) {
    if (current == nullptr) {
        return nullptr;
    }

    if (current->left_is_pair && current->left.p == base) {
        if (current->right_is_pair) {
            return find_right_number(base, current->right.p);
        }
        else {
            return &current->right.i;
        }
    }

    if (current->right_is_pair && current->right.p == base) {
        return find_right_number(current, current->parent);
    }

    if (!current->left_is_pair) {
        return &current->left.i;
    }
    else {
        return find_right_number(base, current->left.p);
    }
}

static bool explode_leftmost_pair(pair* p, int depth = 0) {
    if (depth >= 4) {
        // Explode
        int* left_number = find_left_number(p, p->parent);
        int* right_number = find_right_number(p, p->parent);

        if (left_number && !p->left_is_pair) {
            *left_number += p->left.i;
        }
        if (right_number && !p->right_is_pair) {
            *right_number += p->right.i;
        }

        if (p->parent->left_is_pair && p->parent->left.p == p) {
            p->parent->left_is_pair = false;
            p->parent->left.i = 0;
        }
        else {
            p->parent->right_is_pair = false;
            p->parent->right.i = 0;
        }

        delete p;
    }

    if (p->left_is_pair) {
        if (explode_leftmost_pair(p->left.p, depth + 1)) {
            return true;
        }
    }

    if (p->right_is_pair) {
        if (explode_leftmost_pair(p->right.p, depth + 1)) {
            return true;
        }
    }

    return false;
}

static bool split_leftmost_pair(pair* p) {
    if (p->left_is_pair) {
        if (split_leftmost_pair(p->left.p)) {
            return true;
        }
    }
    else if (p->left.i >= 10) {
        pair* new_pair = new pair();
        new_pair->left_is_pair = false;
        new_pair->left.i = std::floor(p->left.i / 2.0);
        new_pair->right_is_pair = false;
        new_pair->right.i = std::ceil(p->left.i / 2.0);
        new_pair->parent = p;

        p->left_is_pair = true;
        p->left.p = new_pair;
        return true;
    }

    if (p->right_is_pair) {
        if (split_leftmost_pair(p->right.p)) {
            return true;
        }
    }
    else if (p->right.i >= 10) {
        pair* new_pair = new pair();
        new_pair->left_is_pair = false;
        new_pair->left.i = std::floor(p->right.i / 2.0);
        new_pair->right_is_pair = false;
        new_pair->right.i = std::ceil(p->right.i / 2.0);
        new_pair->parent = p;

        p->right_is_pair = true;
        p->right.p = new_pair;
        return true;
    }

    return false;
}

static pair* sum_pairs(pair* a, pair* b) {
    pair* new_root = new pair();
    new_root->parent = nullptr;

    new_root->left_is_pair = true;
    new_root->left.p = a;

    new_root->right_is_pair = true;
    new_root->right.p = b;

    a->parent = new_root;
    b->parent = new_root;

    while (explode_leftmost_pair(new_root) ||
           split_leftmost_pair(new_root));

    return new_root;
}

static long long compute_magnitude(pair* p) {
    long long result = 0;

    if (p->left_is_pair) {
        result += 3 * compute_magnitude(p->left.p);
    }
    else {
        result += 3 * p->left.i;
    }

    if (p->right_is_pair) {
        result += 2 * compute_magnitude(p->right.p);
    }
    else {
        result += 2 * p->right.i;
    }

    return result;
}

static void day18_run_p1() {
    std::ifstream file("../data/day18.txt");

    pair* current_pair = nullptr;

    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            continue;
        }

        std::stringstream stream{ line };

        pair* p = parse_pair(stream);

        if (!current_pair) {
            current_pair = p;
        }
        else {
            current_pair = sum_pairs(current_pair, p);
        }
    }

    long long result = compute_magnitude(current_pair);
    std::cout << "Part 1 Result: " << result << std::endl;
}

static pair* clone_pair(pair* p) {
    pair* new_pair = new pair();
    new_pair->left_is_pair = p->left_is_pair;
    new_pair->right_is_pair = p->right_is_pair;
    new_pair->parent = nullptr;

    if (p->left_is_pair) {
        new_pair->left.p = clone_pair(p->left.p);
        new_pair->left.p->parent = new_pair;
    }
    else {
        new_pair->left.i = p->left.i;
    }

    if (p->right_is_pair) {
        new_pair->right.p = clone_pair(p->right.p);
        new_pair->right.p->parent = new_pair;
    }
    else {
        new_pair->right.i = p->right.i;
    }

    return new_pair;
}

static void day18_run_p2() {
    std::ifstream file("../data/day18.txt");

    std::vector<pair*> pairs;

    while (file.good()) {
        std::string line;
        file >> line;

        if (!file.good()) {
            continue;
        }

        std::stringstream stream{ line };

        pair* p = parse_pair(stream);
        pairs.push_back(p);
    }

    long long highest_magnitude = 0;

    for (pair* a : pairs) {
        for (pair* b : pairs) {
            if (a == b) {
                continue;
            }

            pair* sum = sum_pairs(clone_pair(a), clone_pair(b));
            highest_magnitude = std::max(highest_magnitude, compute_magnitude(sum));
        }
    }

    std::cout << "Part 2 Result: " << highest_magnitude << std::endl;
}

void day18_run() {
    day18_run_p1();
    day18_run_p2();
}
