#ifndef PREVIEW_ITER_RVALUE_REFERENCE_T_HPP
#define PREVIEW_ITER_RVALUE_REFERENCE_T_HPP

#include"../ranges/iter_move.hpp"
///iter_rvalue_reference_t
namespace preview
{
  template< class T ,class=std::enable_if_t<meta::_dereference<T>::value>>
  using iter_rvalue_reference_t =decltype(ranges::iter_move(std::declval<T&>()));
}
#endif //PREVIEW_ITER_RVALUE_REFERENCE_T_HPP
