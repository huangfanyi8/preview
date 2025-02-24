#ifndef PREVIEW_ITERATOR_TYPES_HPP
#define PREVIEW_ITERATOR_TYPES_HPP

#include"../meta/meta.hpp"

namespace preview
{
  template<class T,class=meta::_require_t<meta::_dereference<T>::value>>
  using iter_reference_t = decltype(*std::declval<T&>());
}

#endif //PREVIEW_ITERATOR_TYPES_HPP
