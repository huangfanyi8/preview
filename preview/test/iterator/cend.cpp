#include "../../ranges/cend.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>
#include"../../ranges/find.hpp"

int main()
{
    using namespace preview;
    std::vector vec{3, 1, 4};
    int arr[]{5, 10, 15};
    
    assert(ranges::find(vec, 5) == ranges::cend(vec));
    assert(ranges::find(arr, 5) != ranges::cend(arr));
}