#include "../../ranges/forward_range.hpp"

#include <forward_list>
#include <queue>
#include <ranges>
#include <span>
#include <stack>
#include <tuple>

const char* str{"not a forward range"};
const char str2[] = "a forward range";

static_assert(
    preview::ranges::forward_range<decltype("a forward range")> &&
    !preview::ranges::forward_range<decltype(str)> &&
    preview::ranges::forward_range<decltype(str2)> &&
    !preview::ranges::forward_range<std::stack<char>> &&
    preview::ranges::forward_range<std::forward_list<char>> &&
    !preview::ranges::forward_range<std::tuple<std::forward_list<char>>> &&
    !preview::ranges::forward_range<std::queue<char>> &&
    "");

int main() {}
