#ifndef PREVIEW_INDIRECTLY_READABLE_TRAITS_HPP
#define PREVIEW_INDIRECTLY_READABLE_TRAITS_HPP

#include"../concepts/core.hpp"

///indirectly_readable_traits
namespace preview
{
  namespace _details
  {
    template <int match,class T, class = void>
    struct readable_traits_helper
      :readable_traits_helper<match+1,T>
    {};
    
    template <int match,class T>
    struct readable_traits_helper<match,const T>
      :readable_traits_helper<match,T>
    {};
    
    template <class T>
    struct readable_traits_helper<1,T,std::enable_if_t<std::is_array_v<T>>>
    : std::remove_cv<std::remove_extent_t<T>>
  {};
  
  template <class T>
  struct readable_traits_helper<3,T,std::void_t<std::remove_cv_t<typename T::value_type>,std::remove_cv_t<typename T::element_type>>>
  : std::conditional_t<same_as<std::remove_cv_t<typename T::value_type>,std::remove_cv_t<typename T::element_type>>,
  meta::type_identity<typename T::value_type>,
  meta::type_identity<meta::undefined>>
{};

template <class T>
struct readable_traits_helper<4,T,std::void_t<typename T::value_type>>
:meta::type_identity<typename T::value_type>
{};

template <class T>
struct readable_traits_helper<5,T,std::void_t<typename T::element_type>>
:meta::type_identity<typename T::element_type>
{};

template <class T>
struct readable_traits_helper<6,T>
  :meta::type_identity<meta::undefined>
{};

template <class T>
struct readable_traits_helper<2,T*,std::enable_if_t<std::is_object_v<T>>>
:std::remove_cv<T>
{};
}

template <class T>
struct indirectly_readable_traits
{
private:
  using _base=typename _details::readable_traits_helper<1,T>::type;
public:
  using value_type=std::enable_if_t<!same_as<_base,meta::undefined>,_base>;
};
}
#endif //PREVIEW_INDIRECTLY_READABLE_TRAITS_HPP
