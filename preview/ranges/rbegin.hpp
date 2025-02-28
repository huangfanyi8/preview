#ifndef PREVIEW_RANGES_RBEGIN_HPP
#define PREVIEW_RANGES_RBEGIN_HPP

#include "end.hpp"

///ranges::rbegin
namespace preview::ranges::Cpo
{
    template<class T>
    inline constexpr bool _reversable
        =bidirectional_iterator<decltype(std::declval<_begin&>()(std::declval<T&>()))>
        &&same_as<decltype(std::declval<_end&>()(std::declval<T&>())),decltype(_begin{}(std::declval<T&>()))>;
    //ranges::rbegin
    struct _rbegin
    {
        template<class,class =void >
        struct _has_rbegin
            :std::false_type
        {};
        
        template<class T>
        struct _has_rbegin<T,
            std::enable_if_t<input_or_output_iterator<decltype(decay_copy(std::declval<T>().rbegin()))>>>
            :std::true_type
        {};
        
        template<class T,class = void>
        struct _adl_rbegin
            :std::false_type
        {};
        
        template<class T>
        struct _adl_rbegin<T,
            std::enable_if_t<input_or_output_iterator<decltype(decay_copy(rbegin(std::declval<T&>())))>>>
            :std::bool_constant<meta::_class_or_enum_type<T>>
        {};
    
    private:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (_has_rbegin<T>::value)
                return noexcept(decay_copy(std::declval<T&>().rbegin()));
            else if constexpr (_adl_rbegin<T>::value)
                return noexcept(decay_copy(rbegin(std::declval<T&>())));
            else
            {
                if constexpr (noexcept(_end{}(std::declval<T&>())))
                {
                    using It = decltype(std::declval<_end&>()(std::declval<T&>()));
                    // std::reverse_iterator copy-initializes its member.
                    return std::is_nothrow_copy_constructible_v<It>;
                }
                else
                    return false;
            }
        }
    
    public:
        template<class T,
            class= meta::_require_or_t<
                (std::is_lvalue_reference_v<T> ||
                 ranges::enable_borrowed_range<meta::remove_cvref_t<T>>),
            _has_rbegin<T>::value,
            _adl_rbegin<T>::value,
            _reversable<T>>>
        constexpr auto
        operator()[[nodiscard]](T&& t) const
        noexcept(_can_noexcept<T&>())
        {
            if constexpr (_has_rbegin<T>::value)
                return t.rbegin();
            else if constexpr (_adl_rbegin<T>::value)
                return rbegin(t);
            else
                return std::make_reverse_iterator(_end{}(t));
        }
    };
}
///ranges::crbegin
namespace preview::ranges::Cpo
{
    ///ranges::crbegin
    struct _crbegin
    {
        template<typename T,
            class=std::void_t<decltype(std::declval<_rbegin&>()(static_cast<meta::CT<T>&&>(std::declval<T&>())))>>
        [[nodiscard]]
        constexpr auto operator()(T&& t) const
        noexcept(noexcept(std::declval<_rbegin&>()(static_cast<meta::CT<T>&&>(t))))
        {
            return _rbegin{}(static_cast<meta::CT<T>&&>(t));
        }
    };
}
///cpo
namespace preview::ranges
{
    inline constexpr auto crbegin = Cpo::_crbegin{};
    inline constexpr auto rbegin = Cpo::_rbegin{};
}

#endif //PREVIEW_RANGES_RBEGIN_HPP
