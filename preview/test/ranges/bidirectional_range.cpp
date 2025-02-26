#include "../../ranges/bidirectional_range.hpp"


#include <forward_list>
#include <list>
#include <ranges>
#include <set>
#include <unordered_set>

int main()
{
    static_assert(
        preview::ranges::bidirectional_range<std::set<int>> and
        not preview::ranges::bidirectional_range<std::unordered_set<int>> and
        preview::ranges::bidirectional_range<std::list<int>> and
        not preview::ranges::bidirectional_range<std::forward_list<int>>
    );
}