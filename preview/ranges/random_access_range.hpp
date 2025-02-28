#ifndef PREVIEW_RANDOM_ACCESS_RANGE_HPP
#define PREVIEW_RANDOM_ACCESS_RANGE_HPP

#include "bidirectional_range.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _random_access_range
        :std::false_type
    {};
    
    template<class T>
    struct _random_access_range<T,std::void_t<ranges::iterator_t<T>,
        std::enable_if_t<ranges::bidirectional_range<T> &&random_access_iterator<ranges::iterator_t<T>>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT random_access_range = _details::_random_access_range<T>::value;
}
#endif //PREVIEW_RANDOM_ACCESS_RANGE_HPP
