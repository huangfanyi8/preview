#ifndef PREVIEW_RANGES_INPUT_RANGE_HPP
#define PREVIEW_RANGES_INPUT_RANGE_HPP

#include"range_primitives.hpp"

namespace preview::ranges::_details
{
    template<class T,class = void>
    struct _input_range
        :std::false_type
    {};
    
    template<class T>
    struct _input_range<T,std::void_t<ranges::iterator_t<T>,
        std::enable_if_t<ranges::range<T> && input_iterator<ranges::iterator_t<T>>>>>
        :std::true_type
    {};
}

namespace preview::ranges
{
    template< class T >
    CXX17_CONCEPT input_range = _details::_input_range<T>::value;
}
#endif //PREVIEW_RANGES_INPUT_RANGE_HPP
