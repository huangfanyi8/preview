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
    
    std::cout << a<<'\n';
    
    std::string src {"hello world!\n"};

//  std::ranges::cdata(src)[0] = 'H'; // error, src.data() is treated as read-only
    preview::ranges::data(src)[0] = 'H'; // OK, src.data() is a non-const storage
    
    char dst[20]; // storage for a C-style string
    std::strcpy(dst, preview::ranges::cdata(src));
    // [data(src), data(src) + size(src)] is guaranteed to be an NTBS
    
    std::cout << dst;
}