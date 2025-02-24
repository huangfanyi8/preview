#ifndef PREVIEW_CORE_CONCEPTS_HPP
#define PREVIEW_CORE_CONCEPTS_HPP

#include"../meta/meta.hpp"

///same_as
namespace preview
{
  template<class L,class R>
  CXX17_CONCEPT same_as=std::is_same_v<L,R>&&std::is_same_v<R,L>;
}
///derived_from
namespace preview
{
  namespace _details
  {
    template <class Derived,class Base,class=void>
    struct _derived_from
      :std::false_type
    {};
    
    template<class Derived,class Base>
    struct _derived_from<Derived,Base,std::enable_if_t<std::is_base_of_v<Base, Derived> &&
      std::is_convertible_v<const volatile Derived*, const volatile Base*>>>
    :std::true_type
  {};
}
template<class Derived,class Base>
CXX17_CONCEPT derived_from =_details::_derived_from<Derived,Base>::value;
}
///convertible_to
namespace preview
{
  namespace _details
  {
    template<class From,class To,class = void>
    struct _convertible_to
      : std::false_type
    {};
    
    template<class From,class To>
    struct _convertible_to<From,To,std::void_t<decltype(static_cast<To>(std::declval<From>()))>>
    : std::is_convertible<From, To>
  {};
}

template <class From,class To>
CXX17_CONCEPT  convertible_to =_details::_convertible_to<From,To>::value;
}
///common_reference_with,
///common_with
namespace preview
{
  namespace _details
  {
    template<class,class,class=void>
    struct _common_reference_with
      :std::false_type
    {};
    
    template<class T,class U>
    struct _common_reference_with<T,U,
      std::enable_if_t<same_as<meta::common_reference_t<T, U>,meta::common_reference_t<U,T>>
    &&convertible_to<T, meta::common_reference_t<T, U>>
    &&convertible_to<U, meta::common_reference_t<T, U>>>>
    :std::true_type
  {};
}

template<class T,class U>
CXX17_CONCEPT common_reference_with=_details::_common_reference_with<T,U>::value;

namespace _details
{
  template<class T,class U,class =void >
  struct _common_with
    :std::false_type
  {};
  
  template <class T,class U>
  struct _common_with<T,U,std::void_t<decltype(
  static_cast<std::common_type_t<T, U>>(std::declval<T>()),
    static_cast<std::common_type_t<T, U>>(std::declval<U>()))>>
  :std::true_type
{};
}

template <class T,class U>
CXX17_CONCEPT common_with =
  _details::_common_with<T,U>::value
  &&common_reference_with<std::add_lvalue_reference_t<const T>,std::add_lvalue_reference_t<const U>>
  &&common_reference_with<std::add_lvalue_reference_t<std::common_type_t<T, U>>,
meta::common_reference_t<std::add_lvalue_reference_t<const T>,std::add_lvalue_reference_t<const U>>>;;
}
///integral concepts
namespace preview
{
  template <class T>
  CXX17_CONCEPT integral = std::is_integral_v<T>;
  
  template <class T>
  CXX17_CONCEPT signed_integral = integral<T> && std::is_signed_v<T>;
  
  template <typename T>
  CXX17_CONCEPT unsigned_integral = integral<T> && !signed_integral<T>;
  
  template <typename T>
  CXX17_CONCEPT floating_point = std::is_floating_point_v<T>;
}
///assignable_from
namespace preview
{
  namespace _details
  {
    template<class L,class R,class=void>
    struct _assign_from
      :std::false_type
    {};
    
    template<class L,class R>
    struct _assign_from<L,R,
      std::void_t<decltype(std::declval<L&>()=std::declval<R&&>())>>
    :std::true_type
  {};
}

template< class LHS, class RHS >
CXX17_CONCEPT assignable_from =
  std::is_lvalue_reference_v<LHS>
  &&common_reference_with<const std::remove_reference_t<LHS>&,const std::remove_reference_t<RHS>&>
  &&_details::_assign_from<LHS,RHS>::value;
}
///destructible
///constructible_from
namespace preview
{
  namespace _details
  {
    template<class ,class=void>
    struct _destructible_impl
      :std::false_type
    {};
    
    template<class T>
    struct _destructible_impl<T,std::void_t<decltype(std::declval<T&>().~T())>>
    :std::bool_constant<noexcept(std::declval<T&>().~T())>
  {};
  
  template<class T>
  struct _destructible
    :_destructible_impl<T>
  {};
  
  template<class T>
  struct _destructible<T&>
    :std::true_type
  {};
  
  template<class T>
  struct _destructible<T&&>
    :std::true_type
  {};
  
  template<class T, std::size_t N>
  struct _destructible<T[N]>
    :_destructible_impl<T>
  {};
}
template <class T>
CXX17_CONCEPT destructible = _details::_destructible<T>::value;

template <class T,class... Args>
CXX17_CONCEPT constructible_from = destructible<T> && std::is_constructible_v<T, Args...>;
}
///default_initializable,
///move_constructible,
///copy_constructible
namespace preview
{
  namespace _details
  {
    template <class,class = void>
    struct _default_initializable
      :std::false_type
    {};
    
    template <class T>
    struct _default_initializable <T, std::void_t<decltype(::new T),decltype(T{})>>
    :std::true_type
  {};
}

template <class T>
CXX17_CONCEPT default_initializable =constructible_from<T> &&_details::_default_initializable<T>::value;

template <class T>
CXX17_CONCEPT move_constructible = constructible_from<T, T> && convertible_to<T, T>;

template< class T >
CXX17_CONCEPT copy_constructible =
  move_constructible<T> &&
  constructible_from<T, T&> && convertible_to<T&, T> &&
  constructible_from<T, const T&> && convertible_to<const T&, T> &&
  constructible_from<T, const T> && convertible_to<const T, T>;
}
#endif
