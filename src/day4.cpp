#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "day4.h"

static const int BOARD_SIDE_SIZE = 5;

struct board {
    int values[BOARD_SIDE_SIZE][BOARD_SIDE_SIZE];
    bool markedValues[BOARD_SIDE_SIZE][BOARD_SIDE_SIZE] = { false };

    bool hasWon() {
        int horizontalCounter = 0;
        int verticalCounter = 0;

        for (int i = 0; i < BOARD_SIDE_SIZE; i++) {
            for (int j = 0; j < BOARD_SIDE_SIZE; j++) {
                if (markedValues[i][j]) horizontalCounter++;
                if (markedValues[j][i]) verticalCounter++;
            }

            if (horizontalCounter >= BOARD_SIDE_SIZE ||
                verticalCounter >= BOARD_SIDE_SIZE) {

                return true;
            }

            horizontalCounter = 0;
            verticalCounter = 0;
        }

        return false;
    }

    int calculateScore() {
        int score = 0;

        for (int i = 0; i < BOARD_SIDE_SIZE; i++) {
            for (int j = 0; j < BOARD_SIDE_SIZE; j++) {
                if (!markedValues[i][j]) {
                    score += values[i][j];
                }
            }
        }

        return score;
    }

    void markValue(int value) {
        for (int i = 0; i < BOARD_SIDE_SIZE; i++) {
            for (int j = 0; j < BOARD_SIDE_SIZE; j++) {
                if (values[i][j] == value) {
                    markedValues[i][j] = true;
                }
            }
        }
    }
};

static void day4_run_p1() {
    std::ifstream file("../data/day4.txt");

    std::string numbersStr;
    file >> numbersStr;

    std::vector<int> numbers;
    {
        size_t pos = 0;
        while ((pos = numbersStr.find(",")) != std::string::npos) {
            std::string token = numbersStr.substr(0, pos);
            numbers.push_back(std::stoi(token));

            numbersStr.erase(0, pos + 1);
        }
        numbers.push_back(std::stoi(numbersStr));
    }

    std::vector<board> boards;
    while (file.good()) {
        board curBoard;

        for (int i = 0; i < BOARD_SIDE_SIZE; i++) {
            for (int j = 0; j < BOARD_SIDE_SIZE; j++) {
                file >> curBoard.values[i][j];
            }
        }

        boards.push_back(curBoard);
    }

    board* winningBoard = nullptr;
    int lastNumber = 0;

    for (int number : numbers) {
        lastNumber = number;

        for (board& b : boards) {
            b.markValue(number);

            if (b.hasWon()) {
                winningBoard = &b;
                goto end;
            }
        }
    }

end:

    int result = winningBoard->calculateScore() * lastNumber;
    std::cout << "Part 1 Result: " << result << std::endl;
}

static void day4_run_p2() {
    std::ifstream file("../data/day4.txt");

    std::string numbersStr;
    file >> numbersStr;

    std::vector<int> numbers;
    {
        size_t pos = 0;
        while ((pos = numbersStr.find(",")) != std::string::npos) {
            std::string token = numbersStr.substr(0, pos);
            numbers.push_back(std::stoi(token));

            numbersStr.erase(0, pos + 1);
        }
        numbers.push_back(std::stoi(numbersStr));
    }

    std::vector<board> boards;
    while (file.good()) {
        board curBoard;

        for (int i = 0; i < BOARD_SIDE_SIZE; i++) {
            for (int j = 0; j < BOARD_SIDE_SIZE; j++) {
                file >> curBoard.values[i][j];
            }
        }

        boards.push_back(curBoard);
    }

    board* winningBoard = nullptr;
    int lastNumber = 0;

    for (int number : numbers) {
        lastNumber = number;

        for (int i = boards.size() - 1; i >= 0; i--) {
            board& b = boards[i];
            b.markValue(number);

            if (b.hasWon()) {
                if (boards.size() == 1) {
                    winningBoard = &b;
                    goto end;
                }

                boards.erase(boards.begin() + i);
            }
        }
    }

end:

    int result = winningBoard->calculateScore() * lastNumber;
    std::cout << "Part 2 Result: " << result << std::endl;
}

void day4_run() {
    day4_run_p1();
    day4_run_p2();
}
