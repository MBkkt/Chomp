#include <iostream>
#include <string>
#include "player.h"

Coordinate Player::Move(const Board &board) {
	int row = 0, column = 0;
	do {
		std::cout << "Enter the column and row of your choice separated by a space:" << std::endl;
		std::cin >> column >> row;
	} while (board[row][column] != 1);
	return {row, column};
}

void Player::Swap(std::string &player) {
	if (player == "Robot") {
		player = "Human";
	} else {
		player = "Robot";
	}
}
