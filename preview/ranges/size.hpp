
#ifndef PREVIEW_RANGES_SIZE_HPP
#define PREVIEW_RANGES_SIZE_HPP

#include"rend.hpp"
#include "disable_sized_range.hpp"
#include "limits"

namespace preview::ranges::Cpo
{
    struct _size
    {
    private:
        template<class T,class = void,class = void>
        struct _check_member_size
            :std::false_type
        {};
        
        template<class T>
        struct _check_member_size<T,
            std::void_t<decltype(preview::decay_copy(std::declval<T&>().size()))>,
            std::enable_if_t<integral<decltype(preview::decay_copy(std::declval<T&>().size()))>>>
            :std::true_type
        {};
        
        template<class,class =void >
        struct _adl_size
            :std::false_type
        {};
        
        template<class T>
        struct _adl_size<T,
            meta::_require_t<
            integral<decltype(decay_copy(size(std::declval<T&>())))>,
            meta::_class_or_enum_type<std::remove_reference_t<T>>,
                !disable_sized_range<meta::remove_cvref_t<T>>
            >>
            :std::true_type
        {};
        
        template<class T,class = void>
        struct _sentinel_size
            :std::false_type
        {};
        
        template<class T>
        struct _sentinel_size<T,meta::_require_t<
            forward_iterator<decltype(std::declval<_begin&>()(std::declval<T&>()))>,
            sized_sentinel_for<decltype(std::declval<_end&>()(std::declval<T&>())),decltype(_begin{}(std::declval<T&>()))>,
            std::is_unsigned_v<decltype(std::declval<_end&>()(std::declval<T&>())-std::declval<_begin&>()(std::declval<T&>()))>>>
            :std::false_type
        {};
    private:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (meta::is_bounded_array_v<std::remove_reference_t<T>>)
                return true;
            else if constexpr (_check_member_size<T>::value)
                return noexcept(decay_copy(std::declval<T&>().size()));
            else if constexpr (_adl_size<T>::value)
                return noexcept(decay_copy(size(std::declval<T&>())));
            else if constexpr (_sentinel_size<T>::value)
                return noexcept(_end{}(std::declval<T&>()) - _begin{}(std::declval<T&>()));
        }
    public:
        template<class T,
            class U=std::remove_reference_t<T>,
            class= meta::_require_or_t<meta::is_bounded_array_v<U>,
                _check_member_size<T>::value,
                _adl_size<T>::value,
                _sentinel_size<T>::value>>
        constexpr auto operator()(T&&t)const noexcept(_can_noexcept<T&>())
        {
            if constexpr(meta::is_bounded_array_v<U>)
                return std::extent_v<U>;
            else if constexpr (_check_member_size<T>::value)
                return t.size();
            else if constexpr (_adl_size<T>::value)
                return decay_copy(size(t));
            else if constexpr(_sentinel_size<T>::value)
                return _end{}(std::declval<T&>())- _begin{}(std::declval<T&>());
        }
    };
    
    struct _ssize
    {
        template<class,class = void>
        struct _can_size
            :std::false_type
        {};
        template<class T>
        struct _can_size<T,std::void_t<decltype(std::declval<_size&>()(std::declval<T&>()))>>
            :std::true_type
        {};
        
        template<class T,
            class= std::enable_if_t<_can_size<T>::value>>
        constexpr auto
        operator()[[nodiscard]](T&& t)const noexcept(noexcept(_size{}(t)))
        {
            auto _get_size = _size{}(t);
            using _size_type = decltype(_get_size);
            // Return the wider of ptrdiff_t and make-signed-like-t<__size_type>.
            if constexpr (integral<_size_type>)
            {
                if constexpr (std::numeric_limits<_size_type >::digits< std::numeric_limits<ptrdiff_t>::digits)
                    return static_cast<ptrdiff_t>(_get_size);
                else
                    return static_cast<std::make_signed_t<_size_type>>(_get_size);
            }
            else
                return ptrdiff_t(_get_size);
        }
    };
}

namespace preview::ranges
{
    inline constexpr auto size = Cpo::_size{};
    inline constexpr auto ssize = Cpo::_ssize{};
}
#endif //PREVIEW_RANGES_SIZE_HPP
