#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "day10.h"

struct result_t {
    std::string str;
    bool illegal;
    char illegalChar;
};

static result_t recursive_parse(std::string str, char endCharacter) {
    while (str[0] == '(' || str[0] == '[' || str[0] == '{' || str[0] == '<') {
        result_t r;

        if (str[0] == '(')      r = recursive_parse(str.substr(1), ')');
        else if (str[0] == '[') r = recursive_parse(str.substr(1), ']');
        else if (str[0] == '{') r = recursive_parse(str.substr(1), '}');
        else if (str[0] == '<') r = recursive_parse(str.substr(1), '>');
        else                    r = { str, false, '\0' };

        if (r.illegal) return r;
        str = r.str;

        // Allow incomplete line
        if (str.size() == 0) {
            return { str, false, '\0' };
        }
    }

    // Allow incomplete line
    if (str.size() == 0) {
        return { str, false, '\0' };
    }

    if (str[0] != endCharacter) {
        return { str, true, str[0] };
    }

    return { str.substr(1), false, '\0' };
}

static result_t parse(std::string str) {
    while (str.size() > 0) {
        result_t r;

        if (str[0] == '(')      r = recursive_parse(str.substr(1), ')');
        else if (str[0] == '[') r = recursive_parse(str.substr(1), ']');
        else if (str[0] == '{') r = recursive_parse(str.substr(1), '}');
        else if (str[0] == '<') r = recursive_parse(str.substr(1), '>');
        else                    r = { str, true, str[0] };

        if (r.illegal) return r;
        str = r.str;
    }

    return { str, false, '\0' };
}

static void day10_run_p1() {
    std::ifstream file("../data/day10.txt");

    int result = 0;

    std::string line;
    while (file >> line) {
        result_t r = parse(line);

        if (r.illegal) {
            if (r.illegalChar == ')') result += 3;
            if (r.illegalChar == ']') result += 57;
            if (r.illegalChar == '}') result += 1197;
            if (r.illegalChar == '>') result += 25137;
        }
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day10_run_p2() {
    std::ifstream file("../data/day10.txt");

    std::vector<long long> scores;

    std::string line;
    while (file >> line) {
        result_t r = parse(line);

        if (r.illegal) {
            continue;
        }

        long long score = 0;

        std::string toClose;
        for (char c : line) {
            if (c == '(')      toClose.insert(toClose.begin(), ')');
            else if (c == '[') toClose.insert(toClose.begin(), ']');
            else if (c == '{') toClose.insert(toClose.begin(), '}');
            else if (c == '<') toClose.insert(toClose.begin(), '>');
            else toClose = toClose.substr(1);
        }

        for (char c : toClose) {
            score *= 5;

            if (c == ')')      score += 1;
            else if (c == ']') score += 2;
            else if (c == '}') score += 3;
            else if (c == '>') score += 4;
        }

        scores.push_back(score);
    }

    std::sort(scores.begin(), scores.end());

    std::cout << "Part 2 Result: " << scores[scores.size() / 2] << std::endl;
}

void day10_run() {
    day10_run_p1();
    day10_run_p2();
}
