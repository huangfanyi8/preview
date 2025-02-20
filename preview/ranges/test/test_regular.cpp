#include "../concepts.hpp"
#include <iostream>

template<class T>
// Credit Alexander Stepanov
// concepts are requirements on T
// Requirement on T: T is semiregular
// T a(b); or T a = b; => copy constructor
// T a; => default constructor
// a = b; => assignment
struct Single
{
  static_assert(preview::semiregular<T>);
  T value;
  // Aggregation initialization for Single behaves like following constructor:
  // explicit Single(const T& x) : value(x) {}
  
  // Implicitly declared special member functions behave like following definitions,
  // except that they may have additional properties:
  // Single(const Single& x) : value(x.value) {}
  // Single() {}
  // ~Single() {}
  // Single& operator=(const Single& x) { value = x.value; return *this; }
  // comparison operator is not defined; it is not required by `semiregular` concept
  // bool operator==(Single const& other) const = delete;
};

template<class T>
struct Single1
{
  static_assert(preview::regular<T>);
  T value;
  friend bool operator==(const Single1&x, const Single1&y)
  {return x.value==y.value;}
};

int main()
{
  Single1<int> myInt1{4};
  Single1<int> myInt2;
  myInt2 = myInt1;
  
  if (myInt1 == myInt2)
    std::cout << "Equal\n";
  
  std::cout << myInt1.value << ' ' << myInt2.value << '\n';
}

template<class T>
void print(T const x)
{
  static_assert(preview::semiregular<T>);
  std::cout << x.value << '\n';
}

void test_semiregular()
{
  Single<int> myInt1{4};      // aggregate initialization: myInt1.value = 4
  Single<int> myInt2(myInt1); // copy constructor
  Single<int> myInt3;         // default constructor
  myInt3 = myInt2;            // copy assignment operator
//  myInt1 == myInt2;           // Error: operator== is not defined
  
  print(myInt1); // ok: Single<int> is a `semiregular` type
  print(myInt2);
  print(myInt3);
  
}   // Single<int> variables are destroyed here
