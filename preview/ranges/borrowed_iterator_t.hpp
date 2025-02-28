
#ifndef PREVIEW_RANGES_BORROWED_ITERATOR_T_HPP
#define PREVIEW_RANGES_BORROWED_ITERATOR_T_HPP

#include "dangling.hpp"
#include "range_primitives.hpp"
#include "borrowed_range.hpp"

namespace preview::ranges::_details
{
    template<class R,class = void>
    struct _borrowed_iterator{};
    
    template<class R>
    struct _borrowed_iterator<R,std::void_t<ranges::iterator_t<R>,std::enable_if_t<range<R>&&borrowed_range<R>>>>
    {
        using type = std::conditional_t<ranges::borrowed_range<R>, ranges::iterator_t<R>, ranges::dangling>;
    };
}

namespace preview::ranges
{
    template<class R>
    using borrowed_iterator_t = typename _details::_borrowed_iterator<R>::type;
}
#endif
