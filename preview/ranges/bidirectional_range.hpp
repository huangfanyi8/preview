#ifndef PREVIEW_BIDIRECTIONAL_RANGE_HPP
#define PREVIEW_BIDIRECTIONAL_RANGE_HPP

#include"forward_range.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _bidirectional_range
        :std::false_type
    {};
    
    template<class T>
    struct _bidirectional_range<T,std::void_t<ranges::iterator_t<T>,
        std::enable_if_t<ranges::forward_range<T> && bidirectional_iterator<ranges::iterator_t<T>>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT bidirectional_range = _details::_bidirectional_range<T>::value;
}

#endif //PREVIEW_BIDIRECTIONAL_RANGE_HPP
