#include "robot.h"
#include "player.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#define USING R"(using: "human"/"robot"(who move first) 0<columns<13 0<rows<13)""\n"

int main(int argc, char **argv) {
    if (argc < 3) {
        throw std::runtime_error{USING};
    }
    std::string current_player{argv[1]};
    if (current_player != "Human" && current_player != "Robot") {
        throw std::runtime_error{USING};
    }
    const int COLUMNS = std::stoi(argv[2]);
    const int ROWS = std::stoi(argv[3]);
    if (COLUMNS > Robot::ALGO_LIMIT || ROWS > Robot::ALGO_LIMIT) {
        throw std::runtime_error{USING"Field size exceeds algorithm limit"};
    } else if (1 > COLUMNS || 1 > ROWS) {
        throw std::runtime_error{USING"Field size is too small"};
    }
    std::cout << R"(
 ______     __  __     ______     __    __     ______
/\  ___\   /\ \_\ \   /\  __ \   /\ "-./  \   /\  == \
\ \ \____  \ \  __ \  \ \ \/\ \  \ \ \-./\ \  \ \  _-/
 \ \_____\  \ \_\ \_\  \ \_____\  \ \_\ \ \_\  \ \_\
  \/_____/   \/_/\/_/   \/_____/   \/_/  \/_/   \/_/

)" << "Welcome to Chomp!\n"
      "Choose a square and all squares to the left and upwards will be eaten\n"
      "The computer will do the same\n"
      "The one to eat the poison square loses\n"
      "Good luck!\n" << std::endl;

    std::vector<std::vector<int>> board(ROWS, std::vector<int>(COLUMNS, 1));
    Robot robot{COLUMNS, ROWS};

    for (size_t row = 1, column = 1; row != 0 || column != 0;) {
        Screen::Draw(board);
        if (current_player == "Robot") {
            std::tie(row, column) = robot.move(board);
        } else {
            std::tie(row, column) = Player::Move(board);
        }
        std::cout << current_player << " choose " << row << " " << column << std::endl;
        for (size_t i = row; i < board.size(); ++i) {
            std::fill(board[i].begin() + column, board[i].end(), 0);
        }
        Player::Swap(current_player);
    }
    if (current_player == "Robot") {
        std::cout << "Too bad, the computer wins!" << std::endl;
    } else {
        std::cout << "Yay, you win!" << std::endl;
    }
    return 0;
}
