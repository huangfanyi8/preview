#ifndef PREVIEW_RANGES_SIZED_RANGE_HPP
#define PREVIEW_RANGES_SIZED_RANGE_HPP

#include "size.hpp"
#include "range.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _sized_range
        :std::false_type
    {};
    
    template<class T>
    struct _sized_range<T,std::void_t<std::enable_if_t<ranges::range<T>>,
        decltype(ranges::size(std::declval<T&>()))>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template<class T>
    CXX17_CONCEPT sized_range = _details::_sized_range<T>::value;
}
#endif
