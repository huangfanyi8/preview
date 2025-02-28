
#ifndef PREVIEW_RANGES_VIEW_HPP
#define PREVIEW_RANGES_VIEW_HPP

#include "range.hpp"

namespace preview::ranges
{
    struct view_base { };
    
    template<class T>
    inline constexpr bool enable_view = derived_from<T, view_base> ;
    
    template<class T>
    CXX17_CONCEPT view = ranges::range<T> &&movable<T> && ranges::enable_view<T>;

}

#endif //PREVIEW_RANGES_VIEW_HPP
