#include <algorithm>
#include <iostream>
#include "../../ranges/end.hpp"
#include <vector>
#include "../../ranges/find.hpp"

int main() {
    std::vector<int> vec{3, 1, 4};
    if (preview::ranges::find(vec, 5) != preview::ranges::end(vec))
        std::cout << "found a 5 in vector vec!\n";
    
    int arr[]{5, 10, 15};
    if (preview::ranges::find(arr, 5) != preview::ranges::end(arr));
}
      