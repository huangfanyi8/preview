#include"../../ranges/advance.hpp"

#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    std::vector<int> v {3, 1, 4};
    
    auto vi = v.begin();
    
    preview::ranges::advance(vi, 2);
    std::cout << "1) value: " << *vi << '\n' << std::boolalpha;
    
    preview::ranges::advance(vi, v.end());
    std::cout << "2) vi == v.end(): " << (vi == v.end()) << '\n';
    
    preview::ranges::advance(vi, -3);
    std::cout << "3) value: " << *vi << '\n';
    
    std::cout << "4) diff: " << preview::ranges::advance(vi, 2, v.end())
              << ", value: " << *vi << '\n';
    
    std::cout << "5) diff: " << preview::ranges::advance(vi, 4, v.end())
              << ", vi == v.end(): " << (vi == v.end()) << '\n';
}
