
#ifndef PREVIEW_INDIRECTLY_READABLE_HPP
#define PREVIEW_INDIRECTLY_READABLE_HPP

#include"iter_value_t.hpp"
#include"iter_reference_t.hpp"
#include"iter_rvalue_reference_t.hpp"

///std::indirectly_readable
///iter_const_reference_t
///iter_common_reference_t
namespace preview
{
  namespace _details
  {
    template< class In ,class = void >
    struct _indirectly_readable
      :std::false_type
    {};
    
    template<class In>
    struct _indirectly_readable<In,
      std::void_t<iter_value_t<In>,
    preview::iter_reference_t<In>,
    iter_rvalue_reference_t<In>>>
    :std::bool_constant<common_reference_with<iter_reference_t<In>&&, iter_value_t<In>&>
    &&common_reference_with<iter_reference_t<In>&&, iter_rvalue_reference_t<In>&&>
    &&common_reference_with<iter_rvalue_reference_t<In>&&, const iter_value_t<In>&>
    &&same_as<decltype(*std::declval<const In&>()),iter_reference_t<In>>
      &&same_as<decltype(ranges::iter_move(std::declval<const In&>())),iter_rvalue_reference_t<In>>>
  {};
}

template< class In >
CXX17_CONCEPT indirectly_readable =_details::_indirectly_readable<meta::remove_cvref_t<In>>::value;

}
#endif //PREVIEW_INDIRECTLY_READABLE_HPP
