
#ifndef PREVIEW_ITER_CONST_REFERENCE_T_HPP
#define PREVIEW_ITER_CONST_REFERENCE_T_HPP

#include "iter_value_t.hpp"
#include"iter_reference_t.hpp"
#include"indirectly_readable.hpp"
namespace preview
{
  template< class T ,class=std::enable_if_t<indirectly_readable<T>>>
  using iter_const_reference_t =
    meta::common_reference_t<const iter_value_t<T>&&,iter_reference_t<T>>;
}
#endif //PREVIEW_ITER_CONST_REFERENCE_T_HPP
