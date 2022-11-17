#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "day3.h"

static void day3_run_p1() {
    std::ifstream file("../data/day3.txt");

    std::string firstLine;
    file >> firstLine;

    size_t bitCount = firstLine.size();
    int lineCount = 0;
    int* oneCounts = new int[bitCount];

    file.clear();
    file.seekg(0);

    std::string line;
    while (file >> line) {
        for (size_t i = 0; i < bitCount; i++) {
            oneCounts[i] += (line[i] == '1') ? 1 : 0;
        }

        lineCount += 1;
    }

    int gammaRate = 0;
    int epsilonRate = 0;
    for (size_t i = 0; i < bitCount; i++) {
        int mostCommonBit = (oneCounts[i] > lineCount / 2) ? 1 : 0;

        gammaRate |= mostCommonBit << (bitCount - i - 1);
        epsilonRate |= (1 - mostCommonBit) << (bitCount - i - 1);
    }

    delete[] oneCounts;

    int result = gammaRate * epsilonRate;
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day3_run_p2() {
    std::ifstream file("../data/day3.txt");

    std::string firstLine;
    file >> firstLine;

    size_t bitCount = firstLine.size();
    std::vector<std::string> lines;

    file.clear();
    file.seekg(0);

    std::string line;
    while (file >> line) {
        lines.push_back(line);
    }

    std::vector<std::string> generatorLines = lines;
    std::string matchingGeneratorLine;
    for (size_t i = 0; i < bitCount; i++) {
        int oneCount = 0;

        for (size_t j = 0; j < generatorLines.size(); j++) {
            oneCount += (generatorLines[j][i] == '1') ? 1 : 0;
        }

        int mostCommonBit = (oneCount >= generatorLines.size() / 2.0) ? 1 : 0;

        for (int j = generatorLines.size() - 1; j >= 0; j--) {
            std::string line = generatorLines[j];

            if (line[i] == '1' && mostCommonBit == 0 ||
                line[i] == '0' && mostCommonBit == 1) {

                generatorLines.erase(generatorLines.begin() + j);
            }

            if (generatorLines.size() == 1) {
                matchingGeneratorLine = generatorLines[0];
                goto generatorEnd;
            }
        }
    }

generatorEnd:

    std::vector<std::string> scrubberLines = lines;
    std::string matchingScrubberLine;
    for (size_t i = 0; i < bitCount; i++) {
        int oneCount = 0;

        for (size_t j = 0; j < scrubberLines.size(); j++) {
            oneCount += (scrubberLines[j][i] == '1') ? 1 : 0;
        }

        int leastCommonBit = (oneCount < scrubberLines.size() / 2.0) ? 1 : 0;

        for (int j = scrubberLines.size() - 1; j >= 0; j--) {
            std::string& line = scrubberLines[j];

            if (line[i] == '1' && leastCommonBit == 0 ||
                line[i] == '0' && leastCommonBit == 1) {

                scrubberLines.erase(scrubberLines.begin() + j);
            }

            if (scrubberLines.size() == 1) {
                matchingScrubberLine = scrubberLines[0];
                goto scrubberEnd;
            }
        }
    }

scrubberEnd:

    int generatorResult = std::stoi(matchingGeneratorLine, nullptr, 2);
    int scrubberResult = std::stoi(matchingScrubberLine, nullptr, 2);

    int result = generatorResult * scrubberResult;
    std::cout << "Part 2 Result: " << result << std::endl;
}

void day3_run() {
    day3_run_p1();
    day3_run_p2();
}
