#include <iostream>
#include <limits>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"
#include "day9.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"
#include "day13.h"
#include "day14.h"
#include "day15.h"
#include "day16.h"
#include "day17.h"
#include "day18.h"
#include "day19.h"
#include "day20.h"
#include "day21.h"

int main() {
    std::cout << "Which day do you want to execute? (1-25)" << std::endl;

    int target_day;

    std::cin >> target_day;

    while (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Please input a valid number!" << std::endl;
        std::cin >> target_day;
    }

    switch (target_day) {
    case 1: day1_run(); break;
    case 2: day2_run(); break;
    case 3: day3_run(); break;
    case 4: day4_run(); break;
    case 5: day5_run(); break;
    case 6: day6_run(); break;
    case 7: day7_run(); break;
    case 8: day8_run(); break;
    case 9: day9_run(); break;
    case 10: day10_run(); break;
    case 11: day11_run(); break;
    case 12: day12_run(); break;
    case 13: day13_run(); break;
    case 14: day14_run(); break;
    case 15: day15_run(); break;
    case 16: day16_run(); break;
    case 17: day17_run(); break;
    case 18: day18_run(); break;
    case 19: day19_run(); break;
    case 20: day20_run(); break;
    case 21: day21_run(); break;
    default:
        std::cout << "This day is not yet implemented!" << std::endl;
        break;
    }
}
