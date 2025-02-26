#include"../../ranges/size.hpp"

#include <iostream>
#include <type_traits>
#include <vector>
#include <array>

using namespace preview;

/*测试  ranges::size  ranges:::ssize*/

int main()
{
    
    static_assert(std::is_same_v<preview::contiguous_iterator_tag,preview::_iter_concept<const int*>>);
    auto v = std::vector<int>{};
    std::cout << "ranges::size(v) == " << preview::ranges::size(v) << '\n';
    
    static_assert(!meta::is_bounded_array_v<std::vector<int>>);
    auto il = {7};     // std::initializer_list
    std::cout << "ranges::size(il) == " << preview::ranges::size(il) << '\n';
    
    int array[]{4, 5}; // array has a known bound
    std::cout << "ranges::size(array) == " << ranges::size(array) << '\n';
    
    static_assert(std::is_signed_v<decltype(ranges::size(v))> == false);
    
    std::array arr{1, 2, 3, 4, 5};
    auto s = ranges::ssize(arr);
    
    std::cout << "ranges::ssize(arr) = " << s << '\n'
              << "ranges::ssize is "
              << (std::is_signed_v<decltype(s)> ? "signed" : "unsigned")
              << '\n';
    
    std::cout << "reversed arr: ";
    
    for (--s; s >= 0; --s)
        std::cout << arr[s] << ' ';
    
    std::cout << "\n" "s = " << s << '\n';
}
