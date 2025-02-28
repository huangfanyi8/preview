#ifndef PREVIEW_CONSTANT_RANGE_HPP
#define PREVIEW_CONSTANT_RANGE_HPP

#include "input_range.hpp"

namespace preview::ranges::_details
{
    template< class T ,class = void>
    struct _constant_range
        :std::false_type
    {};
    
    template< class T >
    struct _constant_range<T,std::enable_if_t<ranges::input_range<T>
        &&input_iterator<T>
        &&same_as<iter_const_reference_t<T>, iter_reference_t<T>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template<class T>
    CXX17_CONCEPT common_range = _details::_constant_range<T>::value;
}
#endif //PREVIEW_CONSTANT_RANGE_HPP
