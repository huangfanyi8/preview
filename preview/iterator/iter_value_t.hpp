
#ifndef PREVIEW_ITER_VALUE_T_HPP
#define PREVIEW_ITER_VALUE_T_HPP

#include "iterator_traits.hpp"

///iter_value_t
namespace preview
{
  template<class Iter,class =void>
  struct iter_value
    :meta::type_identity<typename indirectly_readable_traits<meta::remove_cvref_t<Iter>>::value_type >
{};

template<class Iter>
struct iter_value<Iter,std::enable_if_t<_is_specialization_v<meta::remove_cvref_t<Iter>>>>
:meta::type_identity<typename iterator_traits<meta::remove_cvref_t<Iter>>::value_type >
{};

template <class Iter,class Tn = meta::remove_cvref_t<Iter>>
using iter_value_t=typename iter_value<Iter>::type;
}
#endif //PREVIEW_ITER_VALUE_T_HPP
