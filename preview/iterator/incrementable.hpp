#ifndef PREVIEW_INCREMENTABLE_HPP
#define PREVIEW_INCREMENTABLE_HPP

#include "iter_difference_t.hpp"

namespace preview
{
  namespace _details
  {
    template <class T>
    inline constexpr bool _is_non_bool_integral = std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;
    
    template<class T>
    CXX17_CONCEPT _is_integer_like = _is_non_bool_integral<T>;
    
    template<class T>
    CXX17_CONCEPT _is_signed_integer_like = signed_integral<T>;
    
    template<class T,class = void>
    struct _weakly_incrementable
      :std::false_type
    {};
    
    template<class T>
    struct _weakly_incrementable<T,
      std::void_t<iter_difference_t<T>,
    decltype(std::declval<T&>()++),
    meta::_require_t<movable<T>,same_as<decltype(++std::declval<T&>()),T&>>
    >//void_t
    >
    :std::true_type
  {};
  
  template<class T,class = void>
  struct _incrementable:std::false_type
  {};
  
  template<class T>
  struct _incrementable<T,meta::_require_t<_weakly_incrementable<T>::value,
    same_as<decltype(std::declval<T&>()++),T>,
  regular<T>>>
  :std::true_type
{};
}

template <class T>
CXX17_CONCEPT weakly_incrementable =_details::_weakly_incrementable<T>::value;

template <class T>
CXX17_CONCEPT incrementable = _details::_incrementable<T>::value;
}

#endif //PREVIEW_INCREMENTABLE_HPP
