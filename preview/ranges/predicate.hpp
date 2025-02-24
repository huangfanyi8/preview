#ifndef PREVIEW_RANGES_PREDICATE_HPP
#define PREVIEW_RANGES_PREDICATE_HPP

#include "../concepts/comparison.hpp"

namespace preview::ranges
{
  struct identity
  {
    template <typename T>
    constexpr T&& operator()(T&& t) const noexcept
    {
      return std::forward<T>(t);
    }
    using is_transparent = std::true_type;
  };
  
  struct equal_to
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    -> std::enable_if_t<equality_comparable_with<T, U>, bool>
    {return std::forward<T>(t)==std::forward<U>(u);}
    
    using is_transparent = std::true_type;
  };
  
  struct not_equal_to
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    -> std::enable_if_t<equality_comparable_with<T, U>, bool>
    {
      return !equal_to{}(std::forward<T>(t), std::forward<U>(u));
    }
    
    using is_transparent = std::true_type;
  };
  
  namespace _details
  {
    template<class T,class U,class = void>
    struct _less_builtin_ptr_cmp_base
      :std::true_type
    {};
    
    template<class T,class U>
    struct _less_builtin_ptr_cmp_base<T,U,
      std::void_t<decltype(operator<(std::declval<T&&>(), std::declval<U&&>())),
        decltype(std::declval<T&&>().operator<(std::declval<U&&>()))>>
      :std::false_type
    {};
    
    template<class T,class U,class = void,class = void>
    struct _less_builtin_ptr_cmp
      :std::false_type
    {};
    
    template<class T,class U>
    struct _less_builtin_ptr_cmp<T,U
      ,meta::_require_t<
      convertible_to<T,const volatile void*>,
      convertible_to<U,const volatile void*>,
      same_as<bool,decltype(std::declval<T&&>()<std::declval<U&&>())>
      >//require_t
      >
      :_less_builtin_ptr_cmp_base<T,U>
    {};
  }
  
  struct less
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    noexcept(noexcept(std::declval<T>() < std::declval<U>()))
    -> std::enable_if_t<totally_ordered_with<T, U>, bool>
    {
      if constexpr (_details::_less_builtin_ptr_cmp<T, U>::value)
      {
        auto x = reinterpret_cast<long long unsigned int>(
          static_cast<const volatile void*>(std::forward<T>(t)));
        auto y = reinterpret_cast<long long unsigned int>(
          static_cast<const volatile void*>(std::forward<U>(u)));
        return x < y;
      }
      else
      return std::forward<T>(t)<std::forward<U>(u);
    }
    
    using is_transparent = std::true_type;
  };
  
  struct greater
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    noexcept(noexcept(std::declval<U>() < std::declval<T>()))
    -> std::enable_if_t<totally_ordered_with<T, U>, bool>
    {
      return less{}(std::forward<U>(u), std::forward<T>(t));
    }
    
    using is_transparent = std::true_type;
  };
  
  struct greater_equal
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    noexcept(noexcept(std::declval<T>() < std::declval<U>()))
    -> std::enable_if_t<totally_ordered_with<T, U>, bool>
    {
      return !less{}(std::forward<T>(t), std::forward<U>(u));
    }
    
    using is_transparent = std::true_type;
  };
  
  struct less_equal
  {
    template <typename T, typename U>
    constexpr auto operator()(T&& t, U&& u) const
    noexcept(noexcept(std::declval<U>() < std::declval<T>()))
    -> std::enable_if_t<totally_ordered_with<T, U>, bool>
    {
      return !less{}(std::forward<U>(u), std::forward<T>(t));
    }
    
    using is_transparent = std::true_type;
  };
}

#endif
