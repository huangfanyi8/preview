#ifndef PREVIEW_RANGES_REND_HPP
#define PREVIEW_RANGES_REND_HPP

#include "rbegin.hpp"
///ranges::rend
namespace preview::ranges::Cpo
{
    //ranges::rend
    struct _rend
    {
    private:
        //Check rend
        template<class,class =void >
        struct _has_rend
            :std::false_type
        {};
        
        template<class T>
        struct _has_rend<T,
            meta::_require_t<sentinel_for<decltype(preview::decay_copy(rend(std::declval<T&>()))),
                decltype(_rbegin{}(std::forward<T>(std::declval<T&>())))
            >>>
        :std::true_type
        {};
        
        template<class T,class =void >
        struct _adl_rend
            :std::false_type
        {};
        
        template<class T>
        struct _adl_rend<T,
            std::enable_if_t<sentinel_for<
                decltype(decay_copy(std::declval<_rend&>()(std::declval<T&>()))),
                decltype(std::declval<_rbegin&>()(std::declval<T&>()))>>>
            :std::bool_constant<meta::_class_or_enum_type<T>>
        {};
    
    private:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (_has_rend<T>{})
                return noexcept(preview::decay_copy(std::declval<T&>().rend()));
            else if constexpr (_adl_rend<T>::value)
                return noexcept(preview::decay_copy(rend(std::declval<T&>())));
            else
            {
                if constexpr (noexcept(_begin{}(std::declval<T&>())))
                {
                    using It = decltype(_begin{}(std::declval<T&>()));
                    return std::is_nothrow_copy_constructible_v<It>;
                }
                else
                    return false;
            }
        }
    
    public:
        template<class T,
            class=meta::_require_or_t<(std::is_lvalue_reference_v<T> ||ranges::enable_borrowed_range<meta::remove_cvref_t<T>>),
            _has_rend<T>::value,
            _adl_rend<T>::value,
            _reversable<T>>>
        constexpr auto
        operator()[[nodiscard]](T&& t) const
        noexcept(_can_noexcept<T&>())
        {
            if constexpr (_has_rend<T>::value)
                return t.rend();
            else if constexpr (_adl_rend<T>::value)
                return rend(t);
            else
                return std::make_reverse_iterator(_begin{}(t));
        }
    };
}
///ranges::crend
namespace preview::ranges::Cpo
{
    struct _crend
    {
        template<typename T,
            class=std::void_t<decltype(std::declval<_rend&>()(static_cast<meta::CT<T>&&>(std::declval<T&>())))>>
        [[nodiscard]]
        constexpr auto operator()(T&& t) const
        noexcept(noexcept(std::declval<_rend&>()(static_cast<meta::CT<T>&&>(t))))
        {
            return _rend{}(static_cast<meta::CT<T>&&>(t));
        }
    };
}
///cpo
namespace preview::ranges
{
    inline constexpr auto rend = Cpo::_rend{};
    inline constexpr auto crend = Cpo::_crend{};
}
#endif //PREVIEW_RANGES_REND_HPP
