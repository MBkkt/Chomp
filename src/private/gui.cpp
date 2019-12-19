#include "gui.h"
#include <algorithm>
#include <iostream>
#include <numeric>

void Screen::Draw(Board const &board) {
	for (ssize_t row = board.size() - 1; row >= 0; --row) {
		for (ssize_t column = board[row].size() - 1; column >= 0; --column) {
			char c = '#';
			if (row == 0 && column == 0) {
				c = 'p';
			}
			if (board[row][column] == 0) {
				c = ' ';
			}
			std::cout << c << ' ';
		}
		std::cout << row << '\n';
	}
	for (ssize_t column = board.front().size() - 1; column >= 0; --column) {
		std::cout << column << ' ';
	}
	std::cout << std::endl;
}

State Screen::ToState(const Board &board) {
	std::vector<int> state;
	state.reserve(board.size());
	for (auto &row: board) {
		state.push_back(std::accumulate(row.begin(), row.end(), 0));
	}
	return state;
}

Board Screen::Truncation(Board board, int column, int row) {
	for (size_t i = column; i < board.size(); ++i) {
		std::fill(board[i].begin() + row, board[i].end(), 0);
	}
	return board;
}
