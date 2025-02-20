#ifndef PREVIEW_PREDICATE_HPP
#define PREVIEW_PREDICATE_HPP

#include "../concepts/comparison.hpp"

namespace preview
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
  
  struct less
  {
    template <class T,class U>
    constexpr auto operator()(T&& t, U&& u) const
    noexcept(noexcept(std::declval<T>() < std::declval<U>()))
    -> std::enable_if_t<totally_ordered_with<T, U>, bool>
    {
      return std::less<>{}(std::forward<T>(t), std::forward<U>(u));
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
#endif //PREVIEW_PREDICATE_HPP
