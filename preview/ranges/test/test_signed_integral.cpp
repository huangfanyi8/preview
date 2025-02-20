#include "../concepts.hpp"
#include <iostream>
#include <string_view>

template<class  T>
void test(T&& x, std::string_view text = "")
{
  if constexpr (preview::signed_integral<T>)
    std::cout << text << " (" + (text == "") << x << ") is a signed integral\n";
  else if constexpr (preview::unsigned_integral<T>)
    std::cout << text << &" (" [ (text.empty())] << x << ") is an unsigned integral\n";
  else
    std::cout << text << " (" + (text == "") << x << ") is non-integral\n";
}

int main()
{
  test(42);               // signed
  test(0xFULL, "0xFULL"); // unsigned
  test('A');              // platform-dependent
  test(true, "true");     // unsigned
  test(4e-2, "4e-2");     // non-integral (hex-float)
  test("∫∫");             // non-integral
}
