#include"../../ranges/sized_range.hpp"

#include <forward_list>
#include <list>
#include <ranges>

static_assert(preview::ranges::range<std::list<int>>);
static_assert(preview::ranges::range<std::forward_list<int>>);

static_assert
    (
        preview::ranges::sized_range<std::list<int>> and
        !preview::ranges::sized_range<std::forward_list<int>>
    );

int main() {}