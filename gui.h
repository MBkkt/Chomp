#include <vector>

using Board = std::vector<std::vector<int>>;
using State = std::vector<int>;
using Coordinate = std::pair<int, int>;

namespace Screen {
void Draw(const Board &board);

State ToState(const Board &board);

Board Truncation(Board board, int column, int row);
}