#ifndef PREVIEW_ITERATOR_UTILITIES_HPP
#define PREVIEW_ITERATOR_UTILITIES_HPP

#include "indirect_callable_concepts.hpp"
#include "incrementable.hpp"

namespace preview
{
  template<class F,class...Args>
  using indirect_result_t = std::invoke_result_t<F,iter_reference_t<Args>...>;
  
  template<class Iter,
    class Proj,
    class=meta::_require_t<indirectly_readable<Iter>,
      indirectly_regular_unary_invocable<Proj,Iter>>>
  struct projected
  {
    using value_type = meta::remove_cvref_t<indirect_result_t<Proj&, Iter>>;
    indirect_result_t<Proj&, Iter> operator*() const; // not defined
  };
  
  template< class I,
    class Proj,
    class=meta::_require_t<indirectly_regular_unary_invocable<Proj,I>
                           &&indirectly_readable <I>>>
  using projected_value_t =
    meta::remove_cvref_t<std::invoke_result_t<Proj&,iter_value_t<I>&>>;
  
  template<class Iter,class Proj>
  struct incrementable_traits<projected<Iter,Proj>,
    meta::_require_t<weakly_incrementable<Iter>>>
{ using difference_type = iter_difference_t<Iter>; };
}
#endif
