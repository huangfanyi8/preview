#ifndef PREVIEW_RANGES_DATA_HPP
#define PREVIEW_RANGES_DATA_HPP

#include "empty.hpp"

namespace preview::ranges::Cpo
{
    struct _data
    {
    private:
        
        template<class T>
        using _pointer_to_object = std::bool_constant<std::is_object_v<std::remove_pointer_t<T>>&&std::is_pointer_v<T>>;
        
        template<class T ,class = void>
        struct _has_member_data
            :std::false_type
        {};
        
        template<class T>
        struct _has_member_data<T,std::enable_if_t<_pointer_to_object<decltype(preview::decay_copy(std::declval<T&>().data()))>::value>>
            :std::true_type
        {};
        
        template<class T,class = void>
        struct _has_begin_to_contiguous_iterator
            :std::false_type
        {};
        
        template<class T>
        struct _has_begin_to_contiguous_iterator<T,std::void_t<
            decltype(ranges::begin(std::declval<T&>())),
            std::enable_if_t<contiguous_iterator<decltype(ranges::begin(std::declval<T&>()))>>>>
            :std::true_type
        {};
        
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (_has_member_data<T>::value)
                return noexcept(decay_copy(std::declval<T&>().data()));
            else
                return noexcept(std::declval<_begin&>()(std::declval<T&>()));
        }
    
    public:
        template<class T,class=meta::_require_t<(std::is_lvalue_reference_v<T> ||
            ranges::enable_borrowed_range<meta::remove_cvref_t<T>>)>,
            class=meta::_require_or_t<_has_member_data<T>::value,
                _has_begin_to_contiguous_iterator<T>::value>>
        constexpr auto
        operator()[[nodiscard]](T&& t) const noexcept(_can_noexcept<T>())
        {
            if constexpr (_has_member_data<T>::value)
                return t.data();
            else
                return __builtin_addressof(_begin{}(t));
        }
    };
    
    struct _cdata
    {
        template<typename T,
            class=std::void_t<decltype(std::declval<_data&>()(static_cast<meta::CT<T>&&>(std::declval<T&>())))>>
        [[nodiscard]]
        constexpr auto operator()(T&& t) const
        noexcept(noexcept(std::declval<_data&>()(static_cast<meta::CT<T>&&>(t))))
        {
            return _data{}(static_cast<meta::CT<T>&&>(t));
        }
    };
}

namespace preview::ranges
{
    inline constexpr auto data = Cpo::_data{};
    inline constexpr auto cdata = Cpo::_cdata{};
}
#endif //PREVIEW_RANGES_DATA_HPP
