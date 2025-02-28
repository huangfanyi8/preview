
#ifndef PREVIEW_FORWARD_RANGE_HPP
#define PREVIEW_FORWARD_RANGE_HPP

#include "input_range.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _forward_range
        :std::false_type
    {};
    
    template<class T>
    struct _forward_range<T,std::void_t<ranges::iterator_t<T>,
        std::enable_if_t<ranges::input_range<T> && forward_iterator<ranges::iterator_t<T>>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT forward_range = _details::_forward_range<T>::value;
}
#endif //PREVIEW_FORWARD_RANGE_HPP
