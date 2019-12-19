#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

namespace Utils {
inline int Sum(const std::vector<int> &state, ptrdiff_t from = 0, ptrdiff_t to = std::numeric_limits<ptrdiff_t>::max()) {
	return std::accumulate(state.begin() + from, state.begin() + std::min(state.end() - state.begin(), to), 0);
}

inline int Max(const std::vector<int> &state, ptrdiff_t from = 0, ptrdiff_t to = std::numeric_limits<ptrdiff_t>::max()) {
	return *std::max_element(state.begin() + from, state.begin() + std::min(state.end() - state.begin(), to));
}
}