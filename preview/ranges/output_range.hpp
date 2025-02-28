#ifndef PREVIEW_RANGES_OUTPUT_RANGE_HPP
#define PREVIEW_RANGES_OUTPUT_RANGE_HPP

#include "range_primitives.hpp"

namespace preview::ranges::_details
{
    template<class Range, class T,class = void>
    struct _output_range:std::false_type {};
    
    template<class Range, class T>
    struct _output_range<Range,T,
        std::void_t<iterator_t<Range>,std::enable_if_t<range<Range> && output_iterator<iterator_t<Range>, T>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template<class R,class T>
    CXX17_CONCEPT output_range = _details::_output_range<R,T>::value;
}
#endif //PREVIEW_OUTPUT_RANGE_HPP
