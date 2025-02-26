#ifndef PREVIEW_RANGES_RANGE_HPP
#define PREVIEW_RANGES_RANGE_HPP

#include "begin.hpp"
#include "end.hpp"

namespace preview::ranges
{
    namespace _details
    {
        template<class,class = void >
        struct _range:std::false_type {};
        
        template<class T>
        struct _range<T,std::void_t<decltype(ranges::begin(std::declval<T&>())),
            decltype(ranges::end(std::declval<T&>()))>>
            :std::true_type
        {};
    }
    
    template<class T>
    CXX17_CONCEPT range = _details::_range<T>::value;
}

#endif
