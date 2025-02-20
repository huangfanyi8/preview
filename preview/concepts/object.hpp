#ifndef PREVIEW_OBJECT_CONCEPTS_HPP
#define PREVIEW_OBJECT_CONCEPTS_HPP

#include"comparison.hpp"
#include"../ranges/swap.hpp"

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

#endif //PREVIEW_OBJECT_CONCEPTS_HPP
