#include "../../ranges/data.hpp"

#include <cstring>
#include <iostream>
#include <ranges>
#include <string>

int main()
{
    std::string s{"Hello world!\n"};
    
    char a[20]; // storage for a C-style string
    
    static_assert(std::is_lvalue_reference_v<decltype((a))>);
    static_assert(std::is_lvalue_reference_v<decltype((a))>);
    static_assert(std::is_lvalue_reference_v<std::string&>);
    
    std::strcpy(a, preview::ranges::data(s));
    // [data(s), data(s) + size(s)] is guaranteed to be an NTBS
    
    std::cout << a;
}