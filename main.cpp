#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include <tuple>
#include <unordered_set>

constexpr static int ALGO_LIMIT = 12;

#define USING R"(using: "human"/"robot"(who move first) 0<columns<13 0<rows<13)""\n"

using Board = std::vector<std::vector<int>>;
using State = std::vector<int>;
using Coordinate = std::pair<int, int>;

namespace Screen {
void Draw(const Board &board) {
	for (ssize_t row = board.size() - 1; row >= 0; --row) {
		std::cout << row;
		for (ssize_t column = board[row].size() - 1; column >= 0; --column) {
			char c = '#';
			if (row == 0 && column == 0) {
				c = 'p';
			}
			if (board[row][column] == 0) {
				c = ' ';
			}
			std::cout << ' ' << c;
		}
		std::cout << '\n';
	}
	if (!board.empty()) {
		std::cout << ' ';
		for (ssize_t column = board.front().size() - 1; column >= 0; --column) {
			std::cout << ' ' << column;
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

State ToState(const Board &board) {
	std::vector<int> state;
	state.reserve(board.size());
	for (auto &row: board) {
		state.push_back(std::accumulate(row.begin(), row.end(), 0));
	}
	return state;
}

Board Truncation(Board board, int column, int row) {
	for (size_t i = column; i < board.size(); ++i) {
		std::fill(board[i].begin() + row, board[i].end(), 0);
	}
	return board;
}
}

namespace Utils {
int Sum(const State &state, ptrdiff_t from = 0, ptrdiff_t to = std::numeric_limits<ptrdiff_t>::max()) {
	return std::accumulate(state.begin() + from, state.begin() + std::min(state.end() - state.begin(), to), 0);
}

int Max(const State &state, ptrdiff_t from = 0, ptrdiff_t to = std::numeric_limits<ptrdiff_t>::max()) {
	return *std::max_element(state.begin() + from, state.begin() + std::min(state.end() - state.begin(), to));
}
}

struct Robot {
	Robot(int column, int row) {
		std::ifstream in{"./data/states_" + std::to_string(ALGO_LIMIT) + ".txt"};
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

	Coordinate move(const Board &board) {
		Coordinate strategy_optimal;
		auto state = Screen::ToState(board);
		if (!states_.contains(state)) {
			std::vector<Coordinate> strategy_candidates;
			std::vector<State> result_states;
			for (size_t i = 0; i < board.size(); ++i) {
				for (size_t j = 0; j < board[i].size(); ++j) {
					if (board[i][j] == 1) {
						const auto state_truncated = Screen::ToState(Screen::Truncation(board, i, j));
						if (states_.contains(state_truncated)) {
							strategy_candidates.emplace_back(i, j);
							result_states.push_back(state_truncated);
						}
					}
				}
			}
			size_t strategy_optimal_index = 0;
			int min_sum = std::numeric_limits<int>::max();
			for (size_t i = 0; i < strategy_candidates.size(); ++i) {
				auto now_sum = Utils::Sum(result_states[strategy_optimal_index]);
				if (now_sum < min_sum) {
					strategy_optimal_index = i;
					strategy_optimal = strategy_candidates[strategy_optimal_index];
					min_sum = now_sum;
				}
			}
		} else {
			int y_optimal = 0;
			for (int i = 0; i < board.size(); ++i) {
				if (board[i][state[0] - 1] == 1 && i > y_optimal) {
					y_optimal = i;
				}
				strategy_optimal = {y_optimal, state[0] - 1};
			}
		}
		return strategy_optimal;
	}

 private:
	static std::vector<int> Split(std::string line, char delimiter) {
		std::stringstream ss{line};
		std::vector<int> result;
		while (std::getline(ss, line, delimiter)) {
			result.push_back(stoi(line));
		}
		return result;
	}

	struct hash_vector_t {
		size_t operator()(const std::vector<int> &v) const {
			std::hash<int> hasher;
			size_t seed = 0;
			for (int i : v) {
				seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};

	std::unordered_set<State, hash_vector_t> states_;
};

namespace Player {
Coordinate Move(const Board &board) {
	int row = 0, column = 0;
	do {
		std::cout << "Enter the column and row of your choice separated by a space:" << std::endl;
		std::cin >> column >> row;
	} while (board[row][column] != 1);
	return {row, column};
}

void Swap(std::string &player) {
	if (player == "Robot") {
		player = "Human";
	} else {
		player = "Robot";
	}
}
}

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
	if (COLUMNS > ALGO_LIMIT || ROWS > ALGO_LIMIT) {
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
		if (current_player == "robot") {
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
	if (current_player == "robot") {
		std::cout << "Too bad, the computer wins!" << std::endl;
	} else {
		std::cout << "Yay, you win!" << std::endl;
	}
	return 0;
}
