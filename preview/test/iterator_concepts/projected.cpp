#include "../../iterator/single/iterator_concepts.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>

template<class T>
struct Holder
{
    T t;
};

struct Incomplete{};

using P = Holder<Incomplete>*;

static_assert(preview::equality_comparable<P>); // OK
static_assert(preview::indirectly_comparable<P*, P*, std::equal_to<>>); // Error before C++26
static_assert(preview::sortable<P*>); // Error before C++26

int main()
{
    P a[10] = {}; // ten null pointers
    assert(std::count(a, a + 10, nullptr) == 10); // OK
    assert(std::ranges::count(a, a + 10, nullptr) == 10); // Error before C++26
}