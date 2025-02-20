#include "../concepts.hpp"

#include <iostream>
#include <type_traits>

template<class T>
constexpr  auto x2(T&& x)
{
  if constexpr(preview::floting_point<T>)
    return x + x;
  else if constexpr(preview::integral<T>)
    return x<<1;
}


int main()
{
  constexpr auto d = x2(1.1);
  static_assert(std::is_same_v<double const, decltype(d)>);
  std::cout << d << '\n';
  
  constexpr auto f = x2(2.2f);
  static_assert(std::is_same_v<float const, decltype(f)>);
  std::cout << f << '\n';
  
  constexpr auto i = x2(444);
  static_assert(std::is_same_v<int const, decltype(i)>);
  std::cout << i << '\n';
}
