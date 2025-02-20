#ifndef PREVIEW_RANGES_SWAP_HPP
#define PREVIEW_RANGES_SWAP_HPP

#include "../concepts/core.hpp"

///swap
namespace preview
{
  namespace ranges::Cpo
  {
    using std::swap;
    
    template <class T>
    constexpr void swap(T&,T&) = delete;
    
    template <class A, class B,class = void>
    struct _adl_swap
      :std::false_type
    {};
    
    template<class A,class B>
    struct _adl_swap<A,B,std::void_t<decltype(swap(std::declval<A&&>(),std::declval<B&&>()))>>
      :std::bool_constant<meta::_class_or_enum_type<A>||meta::_class_or_enum_type<B>>
    {};
    
    struct _swap
    {
      template <class A, class B,
        class=std::enable_if_t<_adl_swap<A,B>::value>>
      constexpr void operator()(A&& a, B&& b) const
      noexcept(noexcept(swap(static_cast<A&&>(a), static_cast<B&&>(b))))
      {
        swap(static_cast<A&&>(a), static_cast<B&&>(b));
      }
      
      template <class T,
        class=std::enable_if_t<std::negation_v<_adl_swap<T&,T&>>
                               &&move_constructible<T> && assignable_from<T&,T>>>
      constexpr void operator()(T& x,T& y) const
      noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
      {
        auto tmp = static_cast<std::remove_reference_t<T>&&>(x);
        x = static_cast<std::remove_reference_t<T>&&>(y);
        y = static_cast<std::remove_reference_t<T>&&>(tmp);
      }
      
      template <class A, class B, std::size_t Size>
      constexpr void operator()(A (&a)[Size], B (&b)[Size]) const
      noexcept(noexcept(std::declval<const _swap&>()(*a, *b)))
      {
        for (std::size_t i = 0; i < Size; ++i)
          (*this)(a[i], b[i]);
      }
    };
  } // namespace Swap
  
  namespace ranges
  {
    inline constexpr auto swap=ranges::Cpo::_swap{};
  }
  
  namespace _details
  {
    template<class,class=void>
    struct _swappable
      :std::false_type
    {};
    
    template<class T>
    struct _swappable<T,std::void_t<decltype(ranges::swap(std::declval<T&>(),std::declval<T&>()))>>
      :std::true_type
    {};
    
    template<class T,class U,class=void>
    struct _swappable_with
      :std::false_type
    {};
    
    template<class T,class U>
    struct _swappable_with<T,U,std::void_t<decltype(
    ranges::swap(std::declval<T>(), std::declval<T>()),
      ranges::swap(std::declval<U>(), std::declval<U>()),
      ranges::swap(std::declval<T>(), std::declval<U>()),
      ranges::swap(std::declval<U>(), std::declval<T>())
    )>>
      :std::true_type
    {};
    
  }
  
  template<class T>
  CXX17_CONCEPT swappable=_details::_swappable<T>::value;
  
  template< class T, class U >
  CXX17_CONCEPT swappable_with = common_reference_with<T, U>&&_details::_swappable_with<T,U>::value;
}

#endif
