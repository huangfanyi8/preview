
#ifndef PREVIEW_INDIRECTLY_WRITABLE_HPP
#define PREVIEW_INDIRECTLY_WRITABLE_HPP

#include "iter_reference_t.hpp"

///std::indirectly_writable
namespace preview
{
  template< class Out, class T ,class =void>
  struct _indirectly_writable
    :std::false_type
  {};
  
  template< class Out, class T >
  struct _indirectly_writable<Out,T,
    std::void_t<decltype(*std::declval<Out&>()=std::declval<T&&>()),
    decltype(*std::declval<Out&&>() = std::declval<T&&>()),
    decltype(const_cast<const iter_reference_t<Out>&&>(*std::declval<Out&>()) = std::declval<T&&>(),
      const_cast<const iter_reference_t<Out>&&>(*std::declval<Out&>()) = std::declval<T&&>())
  >//void_t
  >
  :std::true_type
{};

template< class Out, class T >
CXX17_CONCEPT indirectly_writable = _indirectly_writable<Out,T>::value;
}
#endif //PREVIEW_INDIRECTLY_WRITABLE_HPP
