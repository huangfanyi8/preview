#ifndef PREVIEW_COMMON_RANGE_HPP
#define PREVIEW_COMMON_RANGE_HPP

#include "range_primitives.hpp"

namespace preview::ranges::_details
{
    template< class T ,class  = void>
    struct _common_range
        :std::false_type
    {};
    
    template< class T >
    struct _common_range<T,std::enable_if_t<ranges::range<T>
        &&same_as<ranges::iterator_t<T>, ranges::sentinel_t<T>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template<class T>
    CXX17_CONCEPT common_range = _details::_common_range<T>::value;
}
#endif //PREVIEW_COMMON_RANGE_HPP
