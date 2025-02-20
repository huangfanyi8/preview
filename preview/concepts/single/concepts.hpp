#ifndef PREVIEW_RANGES_CONCEPTS_HPP
#define PREVIEW_RANGES_CONCEPTS_HPP

#include"../../meta/compatible.hpp"
#include <cstddef>

#if STL_17

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
    
    template<class T, size_t N>
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
///ranges::swap,
///swappable,
///swappable_with
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
    
    template <class A, class B, size_t Size>
    constexpr void operator()(A (&a)[Size], B (&b)[Size]) const
    noexcept(noexcept(std::declval<const _swap&>()(*a, *b)))
    {
      for (size_t i = 0; i < Size; ++i)
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
///boolean_testable
namespace preview
{
  namespace _details
  {
    template<class T, class = void >
    struct _boolean_testable_helper
      :std::false_type
    {};
    
    template<class T>
    struct _boolean_testable_helper<T,std::enable_if_t<preview::convertible_to<decltype(!std::declval<T&&>()), bool>>>
      :std::true_type
    {};
  }
  
  template <class T>
  CXX17_CONCEPT boolean_testable =
    convertible_to<T,bool>&&_details::_boolean_testable_helper<T>::value;
}
///Comparison concepts
namespace preview
{
  namespace _details
  {
    template<class T,class U,class=void>
    struct _equality_comparable_with
      :std::false_type
    {};
    
    template<class T,class U>
    struct _equality_comparable_with<T,U,
      std::enable_if_t<
        boolean_testable<decltype(std::declval<meta::cref_t<T>&>()==std::declval<meta::cref_t<U>&>())>
        &&boolean_testable<decltype(std::declval<meta::cref_t<T>&>()!=std::declval<meta::cref_t<U>&>())>
      >//enable_if_t
    >
      :std::true_type
    {};
    
    template <class A, class B,
      class Ref =meta::common_reference_t<const A&, const B&>>
    CXX17_CONCEPT _comparison_common_type_with_base =
      same_as<Ref, meta::common_reference_t<const B&, const A&>>
      &&(convertible_to<const A&, const Ref&> || convertible_to<A, const Ref&>)
      &&(convertible_to<const B&, const Ref&> || convertible_to<B,const Ref&>);
    
    template <class A, class B>
    CXX17_CONCEPT _comparison_common_type_with =
      _comparison_common_type_with_base<meta::remove_cvref_t<A>, meta::remove_cvref_t<B>>;
    
    template <class T,class U>
    CXX17_CONCEPT _weakly_equality_comparable_with =
      _details::_equality_comparable_with<T,U>::value
      &&_details::_equality_comparable_with<U,T>::value;
  }
  namespace _details
  {
    template<class T,class U,class =int >
    struct _partially_ordered_with_helper
      :std::false_type
    {};
    
    template<class T,class U>
    struct _partially_ordered_with_helper<T,U,meta::_require_t<
      boolean_testable<decltype(std::declval<meta::cref_t<T>>() > std::declval<meta::cref_t<U>>())>,
      boolean_testable<decltype(std::declval<meta::cref_t<T>>() >= std::declval<meta::cref_t<U>>())>,
      boolean_testable<decltype(std::declval<meta::cref_t<T>>() < std::declval<meta::cref_t<U>>())>,
      boolean_testable<decltype(std::declval<meta::cref_t<T>>() <= std::declval<meta::cref_t<U>>())>>>
      :std::true_type
    {};
    
    template <class T,class U>
    CXX17_CONCEPT _partially_ordered_with
      =std::conjunction_v<_partially_ordered_with_helper<U,T>,_details::_partially_ordered_with_helper<T,U>>;
  }
  
  template<class T>
  CXX17_CONCEPT equality_comparable = _details::_equality_comparable_with<T,T>::value;
  
  template<typename T, typename U>
  CXX17_CONCEPT equality_comparable_with
    = equality_comparable<T>
      && equality_comparable<U>
      && common_reference_with<meta::cref_t<T>,meta::cref_t<U>>
      && equality_comparable<meta::common_reference_t<meta::cref_t<T>,meta::cref_t<U>>>
      && _details::_weakly_equality_comparable_with<T,U>
      &&_details::_comparison_common_type_with<T,U>;
  
  template<class T>
  CXX17_CONCEPT totally_ordered= equality_comparable<T>&& _details::_partially_ordered_with<T,T>;
  
  template<class T,class U>
  CXX17_CONCEPT totally_ordered_with
    = totally_ordered<T> && totally_ordered<U>
      && equality_comparable_with<T,U>
      && totally_ordered<meta::common_reference_t<meta::cref_t<T>,meta::cref_t<U>>>
      && _details::_partially_ordered_with<T,U>;
}
///Object concepts
namespace preview
{
  template< class T >
  CXX17_CONCEPT movable
    =std::is_object_v<T>
     &&move_constructible<T> &&assignable_from<T&, T>
     &&swappable<T>;
  
  template< class T >
  CXX17_CONCEPT copyable =
    copy_constructible<T> &&
    movable<T> &&
    assignable_from<T&, T&> &&
    assignable_from<T&, const T&> &&
    assignable_from<T&, const T>;
  
  /*
   *可复制又可默认构造。它满足于与int等内置类型行为相似的类型，除了它们不需要支持与==的比较
   * */
  template< class T >
  CXX17_CONCEPT semiregular = copyable<T> && default_initializable<T>;
  
  /*正则概念指定了一个类型是正则的，
   * 可复制的
   * 默认可构造的
   * 相等可比的
   * 它满足于与int等内置类型行为相似，并且与==相当的类型
   * */
  template<class T>
  CXX17_CONCEPT regular = semiregular<T> && equality_comparable<T>;
  
  template<typename Fn, typename... Args>
  CXX17_CONCEPT invocable = std::is_invocable_v<Fn, Args...>;
  
  template<typename Fn, typename... Args>
  CXX17_CONCEPT regular_invocable = invocable<Fn, Args...>;
  
  template<typename Fn, typename... Args>
  CXX17_CONCEPT predicate = regular_invocable<Fn, Args...>
                            && boolean_testable<std::invoke_result_t<Fn, Args...>>;
  
  template<typename Rel, typename T, typename U>
  CXX17_CONCEPT relation
    = predicate<Rel,T,T> && predicate<Rel, U, U>
      && predicate<Rel,T,U> && predicate<Rel, U, T>;
  
  template<typename Rel, typename T, typename U>
  CXX17_CONCEPT equivalence_relation = relation<Rel, T, U>;
  
  template< class R, class T, class U >
  CXX17_CONCEPT strict_weak_order = relation<R, T, U>;
  
  template<class A,class B>
  CXX17_CONCEPT different_from
    = !same_as<meta::remove_cvref_t<A>,meta::remove_cvref_t<B>>;
}
#else
#error low
#endif//Only STL_17

#endif //PREVIEW_RANGES_CONCEPTS_HPP
