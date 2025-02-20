#ifndef PREVIEW_COMMON_ALGORITHM_HPP
#define PREVIEW_COMMON_ALGORITHM_HPP

#include "utilities.hpp"
#include "indirectly_writable.hpp"
#include "../ranges/predicate.hpp"
#include "../ranges/swap.hpp"
#include "_iterator_concepts.hpp"

namespace preview
{
  template<class In, class Out>
  CXX17_CONCEPT indirectly_movable
    = indirectly_readable<In>
      && indirectly_writable<Out, iter_rvalue_reference_t<In>>;
  
  template<class In, class Out>
  CXX17_CONCEPT indirectly_movable_storable
    = indirectly_movable<In,Out>
      &&indirectly_writable<Out,iter_value_t<In>>
  &&movable<iter_value_t<In>>
    &&constructible_from<iter_value_t<In>, iter_rvalue_reference_t<In>>
  &&assignable_from<iter_value_t<In>&,iter_rvalue_reference_t<In>>;
  
  template<typename In, typename Out>
  CXX17_CONCEPT indirectly_copyable
    = indirectly_readable<In>
      &&indirectly_writable<Out,iter_reference_t<In>>;
  
  template<typename In, typename Out>
  CXX17_CONCEPT indirectly_copyable_storable
    = indirectly_copyable<In, Out>
      &&indirectly_writable<Out, iter_value_t<In>&>
  &&indirectly_writable<Out, const iter_value_t<In>&>
    &&indirectly_writable<Out, iter_value_t<In>&&>
  &&indirectly_writable<Out, const iter_value_t<In>&&>
    &&copyable<iter_value_t<In>>
  &&constructible_from<iter_value_t<In>, iter_reference_t<In>>
    &&assignable_from<iter_value_t<In>&, iter_reference_t<In>>;
  
  namespace _details
  {
  
  }
  template<class A,class B,class = void>
  struct _indirectly_swappable
    :std::false_type
  {};
  
  template<class A,class B>
  struct _indirectly_swappable<A,B,std::void_t<
    decltype(
    std::iter_swap(std::declval<const A&>(),std::declval<const B&>()),
      std::iter_swap(std::declval<const B&>(),std::declval<const A&>()),
      std::iter_swap(std::declval<const B&>(),std::declval<const B&>()),
      std::iter_swap(std::declval<const A&>(),std::declval<const A&>()))>>
    :std::true_type
  {};
  
  //indirectly_swappable`
  template<class A,class B=A>
  CXX17_CONCEPT indirectly_swappable
    = indirectly_readable<A>
      &&indirectly_readable<B>
      &&_indirectly_swappable<A,B>::value;
  
  template<class I1,class I2,class Rel,
    class P1=ranges::identity,
    class P2=ranges::identity>
  CXX17_CONCEPT indirectly_comparable
    = indirect_binary_predicate<Rel,projected<I1,P1>,
      projected<I2,P2>>;
  
  template<class Iter>
  CXX17_CONCEPT permutable
    = forward_iterator<Iter>
      && indirectly_movable_storable<Iter,Iter>
      && indirectly_swappable<Iter,Iter>;
  
  template<class I1,class I2,class Out,
    typename Rel = ranges::less,class P1 = ranges::identity,
    typename P2 = ranges::identity>
  CXX17_CONCEPT mergeable
    = input_iterator<I1>
      && input_iterator<I2>
      && weakly_incrementable<Out>
      && indirectly_copyable<I1,Out>
      && indirectly_copyable<I2,Out>
      && indirect_strict_weak_order<Rel, projected<I1,P1>,projected<I2,P2>>;
  
  template<class Iter,class Rel = ranges::less,
    typename Proj = ranges::identity>
  CXX17_CONCEPT sortable
    = permutable<Iter>
      && indirect_strict_weak_order<Rel,projected<Iter,Proj>>;
  
}
#endif //PREVIEW_COMMON_ALGORITHM_HPP
