
#ifndef PREVIEW_ITER_COMMON_REFERENCE_T_HPP
#define PREVIEW_ITER_COMMON_REFERENCE_T_HPP

#include"indirectly_readable.hpp"

namespace preview
{
  template< class T ,
    class=std::enable_if_t<indirectly_readable<T>>>
  using iter_common_reference_t =
    meta::common_reference_t<iter_reference_t<T>,iter_value_t<T>&>;
}

#endif //PREVIEW_ITER_COMMON_REFERENCE_T_HPP
