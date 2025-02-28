#ifndef PREVIEW_RANGES_END_HPP
#define PREVIEW_RANGES_END_HPP

#include "begin.hpp"

namespace preview::ranges
{
    template<class T>
    using _iterator_t = decltype(std::declval<Cpo::_begin&>()(std::declval<T&>()));
}

namespace preview::ranges::Cpo
{
    //ranges::end
    struct _end
    {
    private:
        template<class,class= void >
        struct _has_member
            :std::false_type
        {};
        
        template<class Class>
        struct _has_member<Class,
            std::enable_if_t<sentinel_for< decltype(decay_copy(std::declval<Class&>().end())),
                _iterator_t<Class>>>>
            :std::true_type
        {};
        
        template<class,class =void >
        struct _adl_end
            :std::false_type
        {};
        
        template<class Class>
        struct _adl_end<Class,
            std::enable_if_t<sentinel_for<
                decltype(decay_copy(end(std::declval<Class&>()))),
                _iterator_t<Class>>
            >//require
        > //_adl_end
            :std::bool_constant<meta::_class_or_enum_type<std::remove_reference_t<Class>>>
        {};
    
    public:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (meta::is_bounded_array_v<std::remove_reference_t<T>>)
            return true;
            else if constexpr (_has_member<T>::value)
                return noexcept(decay_copy(std::declval<T&>().end()));
            else
                return noexcept(decay_copy(end(std::declval<T&>())));
        }
    
    public:
        template<class T,
            class= meta::_require_t<std::is_lvalue_reference_v<T>||enable_borrowed_range<meta::remove_cvref_t<T>>>,
        class= meta::_require_or_t<
        meta::is_bounded_array_v<std::remove_reference_t<T>>,_has_member<T>::value,_adl_end<T>::value>>
        constexpr auto
        operator()[[nodiscard]](T&& t)
        const noexcept(_can_noexcept<T&>())
        {
            if constexpr (meta::is_bounded_array_v<std::remove_reference_t<T>>)
            {
                return t + std::extent_v<std::remove_reference_t<T>>;
            }
            else if constexpr (_has_member<T>::value)
                return decay_copy(t.end());
            else
                return decay_copy(end(t));
        }
    };
}

namespace preview::ranges
{
    inline constexpr auto end = Cpo::_end{};
}
#endif