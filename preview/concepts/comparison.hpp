/*Comparison concepts*/

#ifndef PREVIEW_COMPARISON_CONCEPTS_HPP
#define PREVIEW_COMPARISON_CONCEPTS_HPP

#include "core.hpp"

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

#endif //PREVIEW_COMPARISON_CONCEPTS_HPP
