#ifndef PREVIEW_RANGES_DECAY_COPY_HPP
#define PREVIEW_RANGES_DECAY_COPY_HPP

#include"../meta/meta.hpp"

namespace preview
{
    template <class T>
    constexpr std::decay_t<T> decay_copy(T &&t)
    noexcept(meta::is_nothrow_convertible_v<T, std::decay_t<T>>)
    {
        return std::forward<T>(t);
    }
}
#endif //PREVIEW_RANGES_DECAY_COPY_HPP
