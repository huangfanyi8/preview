#include<cstdio>
#include <atomic>
#include <concepts>
#include <string>
//#include<preview/ranges/concepts.hpp>
#include "../concepts.hpp"

int main()
{
  static_assert(std::is_assignable_v<int&, int>);
  static_assert(preview::assignable_from<int&, int>);
  
  static_assert(std::is_assignable_v<std::string&, std::string>);
  static_assert(preview::assignable_from<std::string&, std::string>);
  
  // Fundamental types don't support assignment to an rvalue
  static_assert(!std::is_assignable_v<int, int>);
  static_assert(!preview::assignable_from<int, int>);
  
  // std::assignable_from doesn't accept all valid assignment expressions:
  
  // rvalue reference assignment
  static_assert(std::is_assignable_v<std::string&&, std::string>);
  static_assert(!preview::assignable_from<std::string&&, std::string>);
  
  // rvalue assignment
  static_assert(std::is_assignable_v<std::string, std::string>);
  static_assert(!preview::assignable_from<std::string, std::string>);
  
  // std::atomic::operator= returns by value
  static_assert(std::is_assignable_v<std::atomic<int>&, int>);
  static_assert(!preview::assignable_from<std::atomic<int>&, int>);
  
  printf("%ld",__cplusplus);
}
