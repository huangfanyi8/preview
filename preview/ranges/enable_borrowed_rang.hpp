#ifndef PREVIEW_RANGES_ENABLE_BORROWED_RANG_HPP
#define PREVIEW_RANGES_ENABLE_BORROWED_RANG_HPP

namespace preview::ranges
{
    template <class>
    inline constexpr auto enable_borrowed_range = false;
}
#endif
