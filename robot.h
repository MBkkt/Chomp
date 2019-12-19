#pragma once

#include <string>
#include <unordered_set>
#include "gui.h"

struct Robot {
	Robot(int column, int row);

	Coordinate move(const Board &board);

	constexpr static int ALGO_LIMIT = 12;

 private:
	static std::vector<int> Split(std::string line, char delimiter);

	struct hash_vector_t {
		size_t operator()(const std::vector<int> &v) const;
	};

	std::unordered_set<State, hash_vector_t> states_;
};
