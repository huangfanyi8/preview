#ifndef PREVIEW_CONTIGUOUS_RANGE_HPP
#define PREVIEW_CONTIGUOUS_RANGE_HPP

#include "random_access_range.hpp"
#include "data.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _contiguous_range
        :std::false_type
    {};
    
    template<class T>
    struct _contiguous_range<T,std::void_t<ranges::iterator_t<T>,
        std::enable_if_t<ranges::random_access_range<T> &&contiguous_iterator<ranges::iterator_t<T>>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT contiguous_range = _details::_contiguous_range<T>::value;
}
#endif //PREVIEW_CONTIGUOUS_RANGE_HPP
