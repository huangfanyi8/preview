
#ifndef PREVIEW_RANGES_CBEGIN_HPP
#define PREVIEW_RANGES_CBEGIN_HPP

#include "begin.hpp"
///ranges::cbegin
namespace preview::ranges::Cpo
{
    struct _cbegin
    {
        template<class T,
            class=std::void_t<decltype(std::declval<_begin&>()(static_cast<meta::CT<T>&&>(std::declval<T&>())))>>
        [[nodiscard]]
        constexpr auto operator()(T&& t) const
        noexcept(noexcept(std::declval<_begin&>()(static_cast<meta::CT<T>&&>(t))))
        {
            return Cpo::_begin()(static_cast<meta::CT<T>&&>(t));
        }
    };
}
///ranges::cbegin
namespace preview::ranges
{
    inline constexpr auto cbegin = Cpo::_cbegin{};
}
#endif //PREVIEW_RANGES_CBEGIN_HPP
