
#ifndef PREVIEW_RANGES_CEND_HPP
#define PREVIEW_RANGES_CEND_HPP

#include "end.hpp"

namespace preview::ranges::Cpo
{
    struct _cend final
    {
        template<typename T,
            class=std::void_t<decltype(std::declval<_end&>()(static_cast<meta::CT<T>&&>(std::declval<T&>())))>>
        [[nodiscard]]
        constexpr auto operator()(T&& t) const
        noexcept(noexcept(std::declval<_end&>()(static_cast<meta::CT<T>&&>(t))))
        {
            return _end{}(static_cast<meta::CT<T>&&>(t));
        }
    };
}
namespace preview::ranges
{
    inline constexpr auto cend = Cpo::_cend{};
}
#endif //PREVIEW_RANGES_CEND_HPP
