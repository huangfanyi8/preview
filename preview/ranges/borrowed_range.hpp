
#ifndef PREVIEW_RANGES_BORROWED_RANGE_HPP
#define PREVIEW_RANGES_BORROWED_RANGE_HPP

#include "range.hpp"

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT borrowed_range =
        ranges::range<T> &&
        (std::is_lvalue_reference_v<T> ||
        ranges::enable_borrowed_range<meta::remove_cvref_t<T>>);
}
#endif
