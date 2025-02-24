
#ifndef PREVIEW_RANGES_DISABLE_SIZED_RANGE_HPP
#define PREVIEW_RANGES_DISABLE_SIZED_RANGE_HPP

namespace preview::ranges
{
    template <class>
    inline constexpr auto disable_sized_range = false;
}

#endif //PREVIEW_RANGES_DISABLE_SIZED_RANGE_HPP
