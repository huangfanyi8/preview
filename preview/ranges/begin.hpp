#ifndef PREVIEW_RANGES_BEGIN_HPP
#define PREVIEW_RANGES_BEGIN_HPP

#include "../iterator/iterator_concepts.hpp"
#include "_decay_copy.hpp"
#include "enable_borrowed_range.hpp"
///ranges::begin
namespace preview::ranges::Cpo
{
    #define DELETE_FUNC(NAME) \
    template<class T>void NAME(T&)=delete; \
    template<class T>void NAME(const T&)=delete;
    
    DELETE_FUNC(begin)
    DELETE_FUNC(end)
    DELETE_FUNC(rbegin)
    DELETE_FUNC(rend)
    DELETE_FUNC(size)
    
    #undef DELETE_FUNC
    
    //ranges::begin
    struct _begin
    {
    private:
        template<class T,class = void>
        struct _has_member
            :std::false_type
        {};
        
        template<class T>
        struct _has_member<T,
            std::enable_if_t<input_or_output_iterator<decltype(decay_copy(std::declval<T>().begin()))>>>
            :std::true_type
        {};
        
        struct _adl_begin
        {
            template<class T>
            auto _valid(T&t)
            -> std::enable_if_t<input_or_output_iterator<decltype(preview::decay_copy(begin(t)))>,std::true_type >
            {}
        };
    
    private:
        template<class T>
        static constexpr bool _can_noexcept()
        {
            if constexpr (std::is_array_v<std::remove_reference_t<T>>)
                return true;
            else if constexpr (_has_member<T>::value)
                return noexcept(preview::decay_copy(std::declval<T&>().begin()));
            else
                return noexcept(preview::decay_copy(begin(std::declval<T&>())));
        }
    
    public:
        template<class T,
            class = std::enable_if_t<std::is_lvalue_reference_v<T>||ranges::enable_borrowed_range<T>>,
            class= meta::_require_or_t<
            meta::_cxx17_requires<_adl_begin,T>,
            std::is_array_v<std::remove_reference_t<T>>,
            _has_member<T>::value>>
        constexpr auto operator()(T&& t)const noexcept(_can_noexcept<T>())
        {
            if constexpr (std::is_array_v<std::remove_reference_t<T>>)
                return t + 0;
            else if constexpr (_has_member<T>::value)
                return decay_copy(t.begin());
            else
                return decay_copy(begin(t));
        }
    };
}
///ranges::begin
namespace preview::ranges
{
    inline constexpr auto begin = Cpo::_begin{};
}

#endif //PREVIEW_RANGES_BEGIN_HPP
