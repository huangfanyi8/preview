#include <ranges>
#include "../../ranges/common_range.hpp"

using namespace preview;
struct A
{
    char* begin();
    char* end();
};
static_assert( ranges::common_range<A> );

struct B
{
    char* begin();
    bool end();
};  // not a common_range: begin/end have different types
static_assert( not ranges::common_range<B> );

struct C
{
    char* begin();
};  // not a common_range, not even a range: has no end()
static_assert( not ranges::common_range<C> );

int main() {}
