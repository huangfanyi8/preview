
#include "../concepts.hpp"

#include <iostream>

template<class T>
void print(const T & i)
{
  if constexpr (preview::integral<T>)
    std::cout << "Integral: " << i << '\n';
  else
    std::cout << "Non-integral: " << i<< '\n';
}


int main()
{
  std::cout << std::boolalpha;
  
  static_assert(preview::integral<bool>);
  print(true);
  
  static_assert(preview::integral<char>);
  print('o');
  
  static_assert(preview::integral<int>);
  print(007);
  
  static_assert( ! preview::integral<double> );
  print(2e2);
  
  static_assert( ! preview::integral<decltype("")> );
  print("∫∫∫");
}
