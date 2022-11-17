#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "day8.h"

static void day8_run_p1() {
    std::ifstream file("../data/day8.txt");

    int result = 0;

    while (file.good()) {
        std::string ignored;
        for (int i = 0; i < 10; i++) {
            file >> ignored;
        }

        std::string separator;
        file >> separator;

        std::string digits;
        for (int i = 0; i < 4; i++) {
            file >> digits;

            if (digits.size() == 2 ||
                digits.size() == 3 ||
                digits.size() == 4 ||
                digits.size() == 7) {

                result += 1;
            }
        }
    }

    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day8_run_p2() {
    std::ifstream file("../data/day8.txt");

    int result = 0;

    while (file.good()) {
        std::string segment_possibilities[7];

        std::string patterns[10];
        std::string pattern1;
        std::string pattern4;
        std::string pattern7;
        std::string pattern8;
        //std::vector<std::string> pattern235;
        std::vector<std::string> pattern069;
        for (int i = 0; i < 10; i++) {
            file >> patterns[i];

            if (patterns[i].size() == 2) {
                pattern1 = patterns[i];
            }
            else if (patterns[i].size() == 3) {
                pattern7 = patterns[i];
            }
            else if (patterns[i].size() == 4) {
                pattern4 = patterns[i];
            }
            else if (patterns[i].size() == 5) {
                //pattern235.push_back(patterns[i]);
            }
            else if (patterns[i].size() == 6) {
                pattern069.push_back(patterns[i]);
            }
            else if (patterns[i].size() == 7) {
                pattern8 = patterns[i];
            }
        }

        if (!file.good()) {
            break;
        }

        {
            segment_possibilities[0] = pattern7;
            segment_possibilities[0].erase(segment_possibilities[0].find(pattern1[0]), 1);
            segment_possibilities[0].erase(segment_possibilities[0].find(pattern1[1]), 1);

            segment_possibilities[1] = pattern4;
            segment_possibilities[1].erase(segment_possibilities[1].find(pattern1[0]), 1);
            segment_possibilities[1].erase(segment_possibilities[1].find(pattern1[1]), 1);
            segment_possibilities[3] = segment_possibilities[1];

            segment_possibilities[2] = pattern1;
            segment_possibilities[5] = pattern1;

            segment_possibilities[4] = pattern8;
            segment_possibilities[4].erase(segment_possibilities[4].find(segment_possibilities[0][0]), 1);
            segment_possibilities[4].erase(segment_possibilities[4].find(segment_possibilities[1][0]), 1);
            segment_possibilities[4].erase(segment_possibilities[4].find(segment_possibilities[1][1]), 1);
            segment_possibilities[4].erase(segment_possibilities[4].find(segment_possibilities[2][0]), 1);
            segment_possibilities[4].erase(segment_possibilities[4].find(segment_possibilities[2][1]), 1);
            segment_possibilities[6] = segment_possibilities[4];

            for (std::string pattern : pattern069) {
                bool a = (pattern.find(pattern1[0]) == std::string::npos);
                bool b = (pattern.find(pattern1[1]) == std::string::npos);

                if (a && !b) {
                    segment_possibilities[2] = pattern1[0];
                    segment_possibilities[5] = pattern1[1];
                }
                else if (b && !a) {
                    segment_possibilities[2] = pattern1[1];
                    segment_possibilities[5] = pattern1[0];
                }

                char missingChar;
                for (char c = 'a'; c <= 'g'; c++) {
                    if (pattern.find(c) == std::string::npos) {
                        missingChar = c;
                        break;
                    }
                }

                if (missingChar == segment_possibilities[4][0] ||
                    missingChar == segment_possibilities[4][1]) {

                    segment_possibilities[4] = missingChar;
                    segment_possibilities[6].erase(segment_possibilities[6].find(missingChar), 1);
                }

                if (missingChar == segment_possibilities[3][0] ||
                    missingChar == segment_possibilities[3][1]) {

                    segment_possibilities[3] = missingChar;
                    segment_possibilities[1].erase(segment_possibilities[1].find(missingChar), 1);
                }
            }
        }

        std::string separator;
        file >> separator;

        std::string digits[4];
        int number = 0;
        for (int i = 0; i < 4; i++) {
            file >> digits[i];

            int digit = -1;

            if (digits[i].size() == 2) {
                digit = 1;
            }
            else if (digits[i].size() == 3) {
                digit = 7;
            }
            else if (digits[i].size() == 4) {
                digit = 4;
            }
            else if (digits[i].size() == 7) {
                digit = 8;
            }
            else {
                bool a = (digits[i].find(segment_possibilities[0][0]) != std::string::npos);
                bool b = (digits[i].find(segment_possibilities[1][0]) != std::string::npos);
                bool c = (digits[i].find(segment_possibilities[2][0]) != std::string::npos);
                bool d = (digits[i].find(segment_possibilities[3][0]) != std::string::npos);
                bool e = (digits[i].find(segment_possibilities[4][0]) != std::string::npos);
                bool f = (digits[i].find(segment_possibilities[5][0]) != std::string::npos);
                bool g = (digits[i].find(segment_possibilities[6][0]) != std::string::npos);

                if (a && b && c && !d && e && f && g)  digit = 0;
                if (a && b && !c && d && e && f && g)  digit = 6;
                if (a && b && c && d && !e && f && g)  digit = 9;
                if (a && !b && c && d && e && !f && g) digit = 2;
                if (a && !b && c && d && !e && f && g) digit = 3;
                if (a && b && !c && d && !e && f && g) digit = 5;
            }

            if (digit == -1) {
                exit(0);
            }

            number += digit * std::pow(10, 4 - i - 1);
        }

        result += number;
    }

    std::cout << "Part 2 Result: " << result << std::endl;
}

void day8_run() {
    day8_run_p1();
    day8_run_p2();
}
