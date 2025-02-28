#ifndef PREVIEW_RANGES_EMPTY_HPP
#define PREVIEW_RANGES_EMPTY_HPP

#include "size.hpp"

namespace preview::ranges::Cpo
{
    struct _empty
    {
    private:
        template<class T,class =void>
        struct _has_empty
            :std::false_type
        {};
        
        template<class T>
        struct _has_empty<T,std::void_t<decltype(bool(std::declval<T&>().empty()))>>
            :std::true_type
        {};
        
        template<class,class = void>
        struct _size_equal_0
            :std::false_type
        {};
        
        template<class T>
        struct _size_equal_0<T,std::void_t<decltype(ranges::size(std::declval<T&>()==0))>>
            :std::true_type
        {};
        
        template<class T,class=void>
        struct _begin_equal_end
            :std::false_type
        {};
        
        template<class T>
        struct _begin_equal_end<T,
            std::void_t<
            std::enable_if_t<forward_iterator<decltype(std::declval<_begin&>()(std::declval<T&>()))>>,
            decltype(bool(std::declval<_begin&>()(std::declval<T&>())==std::declval<_end&>()(std::declval<T&>())))>>
            :std::true_type
        {};
    
    private:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (_has_empty<T>::value)
                return noexcept(bool(std::declval<T&>().empty()));
            else if constexpr (_size_equal_0<T>::value)
                return noexcept(std::declval<_size&>()(std::declval<T&>()) == 0);
            else
                return noexcept(bool(std::declval<_begin&>()(std::declval<T&>())==std::declval<_end&>()(std::declval<T&>())));
        }
    
    public:
        template<class T,
            class= meta::_require_or_t<_has_empty<T>::value,
                _size_equal_0<T>::value,
            _begin_equal_end<T>::value>>
        constexpr bool
        operator()[[nodiscard]](T&& t) const noexcept(_can_noexcept<T&>())
        {
            if constexpr (_has_empty<T>::value)
                return bool(t.empty());
            else if constexpr (_size_equal_0<T>::value)
                return ranges::size(t) == 0;
            else
                return bool(ranges::begin(t) == ranges::end(t));
        }
    };
}

namespace preview::ranges
{inline constexpr auto empty = Cpo::_empty{};}

#endif //PREVIEW_RANGES_EMPTY_HPP
