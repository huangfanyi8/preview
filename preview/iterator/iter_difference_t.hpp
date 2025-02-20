#ifndef PREVIEW_ITER_DIFFERENCE_T_HPP
#define PREVIEW_ITER_DIFFERENCE_T_HPP

#include"iterator_traits.hpp"

///iter_difference_t
namespace preview
{
  template<class Iter,class =void>
  struct iter_difference
    :meta::type_identity<typename incrementable_traits<meta::remove_cvref_t<Iter>>::difference_type >
{};

template<class Iter>
struct iter_difference<Iter,std::enable_if_t<_is_specialization_v<meta::remove_cvref_t<Iter>>>>
:meta::type_identity<typename iterator_traits<meta::remove_cvref_t<Iter>>::difference_type>
{};

template <class Iter,class T=meta::remove_cvref_t<Iter>>
using iter_difference_t = typename iter_difference<T>::type;
}

#endif //PREVIEW_ITER_DIFFERENCE_T_HPP
