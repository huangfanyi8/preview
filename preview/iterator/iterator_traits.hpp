#ifndef PREVIEW_ITERATOR_TRAITS_HPP
#define PREVIEW_ITERATOR_TRAITS_HPP

#include "indirectly_readable_traits.hpp"
#include"incrementable_traits.hpp"
#include "../concepts/object.hpp"
#include"iter_reference_t.hpp"
#include "iterator_base_types.hpp"
#include"iter_difference_t.hpp"

///_iterator traits
namespace preview
{
  namespace _details
  {
    template<class Iter,class =void>
    struct  _cpp17_iterator
      :std::false_type
    {};
    
    template<class Iter>
    struct _cpp17_iterator<Iter,meta::_require_t<
                                meta::_can_reference<decltype(*std::declval<Iter&>())>::value,
      same_as<decltype(++std::declval<Iter&>()),Iter&>,
    meta::_can_reference<decltype(*std::declval<Iter&>()++)>::value>>
    :std::true_type
  {};
  
  template<class Iter>
  CXX17_CONCEPT _is_cpp17_iterator = _cpp17_iterator<Iter>::value&&copyable<Iter>;
  
  template<class Iter,class =void>
  struct _cpp17_input_iterator
    :std::false_type
  {};
  
  template<class _Iter>
  struct _cpp17_input_iterator<_Iter,
    std::void_t<typename incrementable_traits<_Iter>::difference_type,
    typename indirectly_readable_traits<_Iter>::value_type,
    meta::common_reference_t<iter_reference_t<_Iter>&&,typename indirectly_readable_traits<_Iter>::value_type&>,
  meta::common_reference_t<decltype(*std::declval<_Iter&>()++)&&,typename indirectly_readable_traits<_Iter>::value_type&>>>
  :std::bool_constant<_is_cpp17_iterator<_Iter>
                      && equality_comparable<_Iter>
    &&signed_integral<typename incrementable_traits<_Iter>::difference_type>>
{};

template<class Iter>
CXX17_CONCEPT _is_cpp17_input_iterator = _cpp17_input_iterator<Iter>::value;

  template<class Iter,class =void>
  struct _cpp_fwd_iterator
    :std::false_type
  {};
  
  template<class Iter>
  struct _cpp_fwd_iterator<Iter,std::enable_if_t<
                                convertible_to<decltype(std::declval<Iter&>()++),const Iter&>
                                &&same_as<decltype(*std::declval<Iter&>()++), iter_reference_t<Iter>>>>
  :std::true_type
  {};

  template<class Iter>
  CXX17_CONCEPT _is_cpp17_fwd_iterator
    = _is_cpp17_input_iterator<Iter>
      && constructible_from<Iter>
      && std::is_lvalue_reference_v<iter_reference_t<Iter>>
      &&same_as<meta::remove_cvref_t<iter_reference_t<Iter>>,typename indirectly_readable_traits<Iter>::value_type>
  &&_cpp_fwd_iterator<Iter>::value;

  template<class Iter,class =void>
  struct _cpp_bidi_iterator
    :std::false_type
  {};
  
  template<class Iter>
  struct _cpp_bidi_iterator<Iter,std::enable_if_t<
                                 convertible_to<decltype(std::declval<Iter&>()--),const Iter&>
                                 &&same_as<decltype(--std::declval<Iter&>()),Iter&>
    &&same_as<decltype(*std::declval<Iter&>()--), iter_reference_t<Iter>>>>
  :std::true_type
  {};

  template<class Iter>
  CXX17_CONCEPT  _is_cpp17_bidi_iterator = _is_cpp17_fwd_iterator<Iter>&&_cpp_bidi_iterator<Iter>::value;

  template<class Iter,class = void>
  struct _cpp17_random_access_iterator
    :std::false_type
  {};
  
  template<class Iter>
  struct _cpp17_random_access_iterator<Iter,
    meta::_require_t<
    same_as<Iter&, decltype(std::declval<Iter&>()+=std::declval<typename incrementable_traits<Iter>::difference_type&>())>,
    same_as<Iter&, decltype(std::declval<Iter&>()-=std::declval<typename incrementable_traits<Iter>::difference_type&>())>,
    same_as<Iter, decltype(std::declval<Iter&>()+std::declval<typename incrementable_traits<Iter>::difference_type&>())>,
    same_as<Iter, decltype(std::declval<typename incrementable_traits<Iter>::difference_type&>()+std::declval<Iter&>())>,
    same_as<Iter, decltype(std::declval<Iter&>()-std::declval<typename incrementable_traits<Iter>::difference_type&>())>,
    same_as<typename incrementable_traits<Iter>::difference_type,decltype(std::declval<Iter&>()-std::declval<Iter&>())>,
    convertible_to<decltype(std::declval<Iter&>()[std::declval<typename incrementable_traits<Iter>::difference_type&>()]),iter_difference_t<Iter>>
  >>
  :std::true_type
  {};

  template<class Iter>
  CXX17_CONCEPT _is_cpp17_random_access_iterator
    = _is_cpp17_bidi_iterator<Iter>
      && totally_ordered<Iter>
      && _cpp17_random_access_iterator<Iter>::value;

  template<class,class=void>
  struct  _iter_with_nested_types_
    :std::false_type
  {};
  
  template<class Iter>
  struct  _iter_with_nested_types_<Iter,std::void_t<
                                        typename Iter::iterator_category,
    typename Iter::value_type,
    typename Iter::difference_type,
    typename Iter::reference>>
      :std::true_type
  {};

  //检测是否包含类型
  template<class Iter>
  CXX17_CONCEPT _iter_with_nested_types=_iter_with_nested_types_<Iter>::value;

  template<class Iterator,class = void>struct _iterator_traits;
  
  template<class Iterator>
  struct _iterator_traits<Iterator,std::enable_if_t<_iter_with_nested_types<Iterator>>>
  {
  private:
  template<class,class=void>
  struct _traits_ptr
  { using type = void; };
  
  template<class Iter>
  struct _traits_ptr<Iter,std::void_t<typename Iter::pointer>>
  { using type = typename Iter::pointer; };
  
  public:
  using iterator_category = typename Iterator::iterator_category;
  using value_type	      = typename Iterator::value_type;
  using difference_type   = typename Iterator::difference_type;
  using pointer	      = typename _traits_ptr<Iterator>::type;
  using reference	      = typename Iterator::reference;
  };

  template<class Iterator>
  struct _iterator_traits<Iterator,std::enable_if_t<!_iter_with_nested_types<Iterator>&&_is_cpp17_input_iterator<Iterator>>>
  {
  private:
  template<int tag,class Iter,class =void >
  struct _traits_category
    :_traits_category<tag+1,Iter>
  {};
  
  template<class _Iter>
  struct _traits_category<1,_Iter,std::void_t<typename _Iter::iterator_category>>
  { using type = typename _Iter::iterator_category; };
  
  template<class _Iter>
  struct _traits_category<2,_Iter,std::enable_if_t<_is_cpp17_random_access_iterator<_Iter>>>
  { using type = random_access_iterator_tag; };
  
  template<class _Iter>
  struct _traits_category<3,_Iter,std::enable_if_t<_is_cpp17_bidi_iterator<_Iter>>>
  { using type = bidirectional_iterator_tag; };
  
  template<class _Iter>
  struct _traits_category<4,_Iter,std::enable_if_t<_is_cpp17_fwd_iterator<_Iter>>>
  { using type = forward_iterator_tag; };
  
  template<class Iter>
  struct _traits_category<5,Iter>
  {using type=input_iterator_tag;};
  
  template<int model,class Iter,class=void>
  struct _traits_ptr
    :_traits_ptr<model+1,Iter>
  {
  };
  
  template<class Iter>
  struct _traits_ptr<1,Iter,std::void_t<typename Iter::pointer>>
  { using type = typename Iter::pointer; };
  
  template<class Iter>
  struct _traits_ptr<2,Iter,std::void_t<decltype(std::declval<Iter&>().operator->())>>
  { using type = decltype(std::declval<Iter&>().operator->()); };
  
  template<class Iter>
  struct _traits_ptr<3,Iter>
  {
    using type=void;
  };
  
  template<class Iter,class =void >
  struct _traits_ref
  { using type = iter_reference_t<Iter>; };
  
  template<class Iter>
  struct _traits_ref<Iter,std::void_t<typename Iter::reference>>
  { using type = typename Iter::reference; };
  
  public:
  using iterator_category = typename _traits_category<1,Iterator>::type;
  using value_type
    = typename indirectly_readable_traits<Iterator>::value_type;
  using difference_type
    = typename incrementable_traits<Iterator>::difference_type;
  using pointer	      = typename _traits_ptr<1,Iterator>::type;
  using reference	      = typename _traits_ref<Iterator>::type;
  };

  template<class Iterator>
  struct _iterator_traits<Iterator,
    std::enable_if_t<!_iter_with_nested_types<Iterator>
    &&_is_cpp17_iterator<Iterator>
  &&!_is_cpp17_input_iterator<Iterator>>>
  {
  private:
  template<class Iter,class= void>
  struct _traits_diff
  { using type = void; };
  
  template<class Iter>
  struct _traits_diff<Iter,
    std::void_t<typename incrementable_traits<Iter>::difference_type>>
  {using type = typename incrementable_traits<Iter>::difference_type;};

  public:
  using iterator_category = output_iterator_tag;
  using value_type	      = void;
  using difference_type   = typename _traits_diff<Iterator>::type;
  using pointer	      = void;
  using reference	      = void;
  };
}

}//end
///iterator_traits
namespace  preview
{
  template<class Iterator,class=void>
  struct iterator_traits
    :public _details::_iterator_traits<Iterator,void>
  { };
  
  template<class T>
  struct iterator_traits<T*>
  {
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
  };
  
  template<class T>
  struct iterator_traits<const T*>
  {
    typedef random_access_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;
  };
  
  //检查iterator_traits 是否被特化
  template<class Iter>
  inline constexpr bool  _is_specialization_v
    =!std::is_base_of_v<_details::_iterator_traits<Iter>,iterator_traits<Iter>>;
}

#endif //PREVIEW_ITERATOR_TRAITS_HPP
