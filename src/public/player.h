#pragma once

#include <string>
#include "gui.h"

namespace Player {
Coordinate Move(const Board &board);

void Swap(std::string &player);
}