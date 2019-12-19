#include <fstream>
#include <sstream>
#include "robot.h"
#include "utils.h"

Robot::Robot(int column, int row) {
    std::ifstream in{"./states_" + std::to_string(ALGO_LIMIT) + ".txt"};
    std::vector<State> states;
    for (std::string line; std::getline(in, line);) {
        states.push_back(Split(line, ' '));
    }
    for (auto &&state : states) {
        if (Utils::Max(state) <= column && Utils::Sum(state, row) == 0) {
            state.resize(row);
            states_.insert(std::move(state));
        }
    }
}

Coordinate Robot::move(const Board &board) {
    Coordinate optimal_move;
    auto state = Screen::ToState(board);
    if (!states_.contains(state)) {
        std::vector<Coordinate> move_candidates;
        std::vector<State> state_candidates;
        for (size_t i = 0; i < board.size(); ++i) {
            for (size_t j = 0; j < board[i].size(); ++j) {
                if (board[i][j] == 1) {
                    const auto state_truncated = Screen::ToState(Screen::Truncation(board, i, j));
                    if (states_.contains(state_truncated)) {
                        move_candidates.emplace_back(i, j);
                        state_candidates.push_back(state_truncated);
                    }
                }
            }
        }
        size_t optimal_move_index = 0;
        int optimal_move_sum = std::numeric_limits<int>::max();
        for (size_t i = 0; i < move_candidates.size(); ++i) {
            auto current_move_sum = Utils::Sum(state_candidates[optimal_move_index]);
            if (current_move_sum < optimal_move_sum) {
                optimal_move_index = i;
                optimal_move = move_candidates[optimal_move_index];
                optimal_move_sum = current_move_sum;
            }
        }
    } else {
        int optimal_y = 0;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i][state[0] - 1] == 1 && i > optimal_y) {
                optimal_y = i;
            }
            optimal_move = {optimal_y, state[0] - 1};
        }
    }
    return optimal_move;
}

std::vector<int> Robot::Split(std::string line, char delimiter) {
    std::stringstream ss{line};
    std::vector<int> result;
    while (std::getline(ss, line, delimiter)) {
        result.push_back(stoi(line));
    }
    return result;
}

size_t Robot::hash_vector_t::operator()(const std::vector<int> &v) const {
    std::hash<int> hasher;
    size_t seed = 0;
    for (int i : v) {
        seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}
