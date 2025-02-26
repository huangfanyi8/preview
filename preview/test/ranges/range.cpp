#include"../../ranges/range.hpp"
struct SimpleRange
{
    int* begin();
    int* end();
};
static_assert(preview::ranges::range<SimpleRange>);

// Not a range: no begin/end
struct NotRange
{
    int t {};
};
static_assert(!preview::ranges::range<NotRange>);

// Not a range: begin does not return an input_or_output_iterator
struct NotRange2
{
    void* begin();
    int* end();
};

static_assert(!preview::ranges::range<NotRange2>);

int main() {}