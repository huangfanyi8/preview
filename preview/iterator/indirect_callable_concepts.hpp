

#ifndef PREVIEW_INDIRECT_CALLABLE_CONCEPTS_HPP
#define PREVIEW_INDIRECT_CALLABLE_CONCEPTS_HPP

#include "indirectly_readable.hpp"
#include "iter_common_reference_t.hpp"
#include "../concepts/comparison.hpp"

namespace preview
{
  template<class Fn,class Iter>
  CXX17_CONCEPT indirectly_unary_invocable
    = indirectly_readable<Iter>
      && copy_constructible<Fn>
      && invocable<Fn&,iter_value_t<Iter>&>
      && invocable<Fn&,iter_reference_t<Iter>>
      && invocable<Fn&,iter_common_reference_t<Iter>>
  &&common_reference_with<std::invoke_result_t<Fn&,iter_value_t<Iter>&>,
    std::invoke_result_t<Fn&,iter_reference_t<Iter>>>;
  
  template<typename Fn, typename Iter>
  CXX17_CONCEPT indirectly_regular_unary_invocable
    = indirectly_readable<Iter>
      &&copy_constructible<Fn>
      &&regular_invocable<Fn&, iter_value_t<Iter>&>
      &&regular_invocable<Fn&, iter_reference_t<Iter>>
      &&regular_invocable<Fn&, iter_common_reference_t<Iter>>
  &&common_reference_with<std::invoke_result_t<Fn&, iter_value_t<Iter>&>,std::invoke_result_t<Fn&, iter_reference_t<Iter>>>;
  
  template<typename Fn, typename Iter>
  CXX17_CONCEPT indirect_unary_predicate = indirectly_readable<Iter>
                                           && copy_constructible<Fn> && predicate<Fn&, iter_value_t<Iter>&>
                                           && predicate<Fn&, iter_reference_t<Iter>>
                                           && predicate<Fn&, iter_common_reference_t<Iter>>;
  
  template<typename _Fn, typename _I1, typename _I2>
  CXX17_CONCEPT indirect_binary_predicate
    = indirectly_readable<_I1> && indirectly_readable<_I2>
      && copy_constructible<_Fn>
      && predicate<_Fn&, iter_value_t<_I1>&, iter_value_t<_I2>&>
      && predicate<_Fn&, iter_value_t<_I1>&, iter_reference_t<_I2>>
      && predicate<_Fn&, iter_reference_t<_I1>, iter_value_t<_I2>&>
      && predicate<_Fn&, iter_reference_t<_I1>, iter_reference_t<_I2>>
      && predicate<_Fn&, iter_common_reference_t<_I1>,
    iter_common_reference_t<_I2>>;
  
  template<class F,class I1,class I2 = I1>
  CXX17_CONCEPT indirect_equivalence_relation
    = indirectly_readable<I1>
      && indirectly_readable<I2>
      && copy_constructible<F>
      && equivalence_relation<F&, iter_value_t<I1>&, iter_value_t<I2>&>
      && equivalence_relation<F&, iter_value_t<I1>&, iter_reference_t<I2>>
      && equivalence_relation<F&, iter_reference_t<I1>, iter_value_t<I2>&>
      && equivalence_relation<F&, iter_reference_t<I1>,iter_reference_t<I2>>
      && equivalence_relation<F&, iter_common_reference_t<I1>,iter_common_reference_t<I2>>;
  
  template<class F,class I1,class I2 = I1>
  CXX17_CONCEPT indirect_strict_weak_order
    = indirectly_readable<I1>
      && indirectly_readable<I2>
      && copy_constructible<F>
      && strict_weak_order<F&, iter_value_t<I1>&, iter_value_t<I2>&>
  && strict_weak_order<F&, iter_value_t<I1>&, iter_reference_t<I2>>
    && strict_weak_order<F&, iter_reference_t<I1>, iter_value_t<I2>&>
  && strict_weak_order<F&, iter_reference_t<I1>, iter_reference_t<I2>>
    && strict_weak_order<F&, iter_common_reference_t<I1>,iter_common_reference_t<I2>>;
  
}
#endif //PREVIEW_INDIRECT_CALLABLE_CONCEPTS_HPP
