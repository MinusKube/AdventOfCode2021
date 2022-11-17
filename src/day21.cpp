#include <iostream>
#include <fstream>
#include <vector>

#include "day21.h"

struct player {
    int position;
    int score;
};

constexpr int POSITION_OFFSET = 28;

static int roll_dice(int side_count, int last_dice_roll) {
    return (last_dice_roll % side_count) + 1;
}

static int move_forward(int position, int amount) {
    int new_position = position + amount;

    while (new_position > 10) {
        new_position -= 10;
    }

    return new_position;
}

static void day21_run_p1() {
    std::ifstream file("../data/day21.txt");

    player players[2];

    while (file.good()) {
        std::string ignored_word;

        int index;
        int value;

        file >> ignored_word >> index >> ignored_word >> ignored_word >> value;

        if (!file.good()) {
            continue;
        }

        players[index - 1].position = value;
        players[index - 1].score = 0;
    }

    int winner = -1;
    int last_dice_roll = 0;
    int dice_roll_count = 0;
    int current_turn = 0;

    while (winner == -1) {
        int roll_sum = 0;
        for (int i = 0; i < 3; i++) {
            int roll = roll_dice(100, last_dice_roll);
            roll_sum += roll;

            last_dice_roll = roll;
            dice_roll_count += 1;
        }

        player& p = players[current_turn];

        p.position = move_forward(p.position, roll_sum);
        p.score += p.position;

        if (players[current_turn].score >= 1000) {
            winner = current_turn;
        }

        current_turn = (current_turn + 1) % 2;
    }

    int loser = (winner + 1) % 2;

    int result = players[loser].score * dice_roll_count;
    std::cout << "Part 1 Result: " << result << std::endl;
}

struct universe {
    player players[2];
    int current_turn;
};

static void day21_run_p2() {
    std::ifstream file("../data/day21_example.txt");

    player original_players[2];

    while (file.good()) {
        std::string ignored_word;

        int index;
        int value;

        file >> ignored_word >> index >> ignored_word >> ignored_word >> value;

        if (!file.good()) {
            continue;
        }

        original_players[index - 1].position = value;
        original_players[index - 1].score = 0;
    }

    universe* default_universe = new universe;
    default_universe->players[0] = original_players[0];
    default_universe->players[1] = original_players[1];
    default_universe->current_turn = 0;

    long long player_wins[2] = { 0 };

    std::vector<universe*> universes;
    universes.push_back(default_universe);

    long potato = time(nullptr);

    while (universes.size() > 0) {
        if (std::abs(time(nullptr) - potato) > 1) {
            potato = time(nullptr);
            std::cout << universes.size() << " (0) " << player_wins[0] << " (1) " << player_wins[1] << std::endl;
        }

        universe& u = *universes[0];

        if (u.players[0].score >= 21) {
            player_wins[0] += 1;

            delete &u;
            universes.erase(universes.begin());
            continue;
        }
        else if (u.players[1].score >= 21) {
            player_wins[1] += 1;

            delete &u;
            universes.erase(universes.begin());
            continue;
        }

        for (int roll_sum = 3; roll_sum <= 9; roll_sum++) {
            // Keep current universe as the universe for the sum "3"
            universe* new_u = new universe;

            if (roll_sum > 0) {
                universes.push_back(new_u);

                //std::cout << "B -> " << u.current_turn << std::endl;

                //new_u = &universes.back();
                new_u->players[0] = u.players[0];
                new_u->players[1] = u.players[1];
                new_u->current_turn = u.current_turn;
            }

            player& p = new_u->players[new_u->current_turn];

            p.position = move_forward(p.position, roll_sum);
            p.score += p.position;

            new_u->current_turn = (new_u->current_turn + 1) % 2;
        }

        delete &u;
        universes.erase(universes.begin());
    }

    std::cout << "0: " << player_wins[0] << std::endl;
    std::cout << "1: " << player_wins[1] << std::endl;

    std::cout << "Part 2 Result: " << 0 << std::endl;
}

void day21_run() {
    day21_run_p1();
    day21_run_p2();
}
