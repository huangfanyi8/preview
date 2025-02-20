#ifndef PREVIEW_INCREMENTABLE_TRAITS_HPP
#define PREVIEW_INCREMENTABLE_TRAITS_HPP

#include "../concepts/core.hpp"

///incrementable_traits
namespace preview
{
  namespace _details
  {
    //计算类型 I 所关联的差类型
    template <int tag,class T,class U= void>
    struct _incrementable_traits
      :_incrementable_traits<tag+1,T,U>
    {};
    
    template <class T>
    struct _incrementable_traits<1,T*,std::enable_if_t<std::is_object_v<T>>>
    : meta::type_identity<std::ptrdiff_t >
  {};
  
  template <int tag,class T>
  struct _incrementable_traits<tag,const T>
    :_incrementable_traits<tag,T>
  {};
  
  template <class T>
  struct _incrementable_traits<2,T,std::void_t<typename T::difference_type>>
  :meta::type_identity<typename T::difference_type>
{};

template <typename T>
struct _incrementable_traits<3,T,
  std::enable_if_t<integral<decltype(std::declval<const T&>()-std::declval<const T&>())>>>
:std::make_signed<decltype(std::declval<T>() - std::declval<T>())>
{};

template <class T>
struct _incrementable_traits<4,T>
{};

}

template <class T,class=void>
struct incrementable_traits
{
  using difference_type=typename _details::_incrementable_traits<1,T>::type;
};
}
#endif //PREVIEW_INCREMENTABLE_TRAITS_HPP
