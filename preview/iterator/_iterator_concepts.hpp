
#ifndef PREVIEW__ITERATOR_CONCEPTS_HPP
#define PREVIEW__ITERATOR_CONCEPTS_HPP

#include "incrementable.hpp"
#include "indirectly_readable.hpp"
#include "indirectly_writable.hpp"
#include "_iter_concept.hpp"

///iterator concepts
namespace preview
{
  template<class T,class = void>
  struct _input_or_output
    :std::false_type
  {};
  
  template<class T>
  struct _input_or_output<T, std::enable_if_t<meta::_can_reference<decltype(*std::declval<T&>())>::value
                             &&weakly_incrementable<T>>>
  :std::true_type
{};

template <class Iter>
CXX17_CONCEPT input_or_output_iterator = _input_or_output<Iter>::value;

template <class S,class I>
CXX17_CONCEPT sentinel_for =
  semiregular<S>
  && input_or_output_iterator<I>
  &&_details::_weakly_equality_comparable_with<S, I>;

template <class S, class I,class=void>
inline constexpr bool disable_sized_sentinel_for = false;

template <class S, class I, class = void>
struct _sized_sentinel_for
  :std::false_type
{};

template <class S, class I>
struct _sized_sentinel_for<S,I,
  meta::_require_t<same_as<decltype(std::declval<const S&>() - std::declval<const I&>()), iter_difference_t<I>>>>
:std::true_type
{};

template <class S, class I>
CXX17_CONCEPT sized_sentinel_for =
  sentinel_for<S, I>
  &&!disable_sized_sentinel_for<std::remove_cv_t<S>, std::remove_cv_t<I>>
&&_sized_sentinel_for< S, I>::value;

namespace _details
{
  template<class Iter,class = void>
  inline constexpr bool _is_input_iterator=false;
  
  template<class Iter>
  inline constexpr bool _is_input_iterator<Iter,
    std::void_t<meta::_require_t<indirectly_readable<Iter>,
    input_or_output_iterator<Iter>>,
  _iter_concept<Iter>,
  meta::_require_t<derived_from<_iter_concept<Iter>,input_iterator_tag>>>> =true;
}

template<class Iter>
CXX17_CONCEPT input_iterator= _details::_is_input_iterator<Iter>;

template<int model=1>
struct _iterator_category_concepts
{
  template<class I,class T,class= void>
  struct _impl
    :std::false_type
  {};
  
  template<class I,class T>
  struct _impl<I,T,std::void_t<decltype(*std::declval<I&>() = std::declval<T&&>())>>
  :std::true_type
  {};
  
};

template<>
struct _iterator_category_concepts<2>
{
  template<class I,class = void>
  struct _impl
    :std::false_type
  {};
  
  template<class I>
  struct _impl<I,std::void_t<std::enable_if_t<same_as<decltype(--std::declval<I&>()),I&>&&same_as<decltype(std::declval<I&>()--),I>>>>
  :std::true_type
  {};
};

template<>
struct _iterator_category_concepts<3>
{
private:
  template<class I>
  using _iter_diff_t = const iter_difference_t<I>&;

public:
  template<class I,class = void>
  struct _impl:std::false_type
  {};
  
  template<class I>
  struct _impl<I,std::void_t<
                 meta::_require_t<same_as<decltype(std::declval<I&>() += std::declval<_iter_diff_t<I>>()), I&>>,
  meta::_require_t<same_as<decltype(std::declval<I&>() -= std::declval<_iter_diff_t<I>>()), I&>>,
  meta::_require_t<same_as<I, decltype(std::declval<const I&>()+std::declval<const iter_difference_t<I>&>())>>,
  meta::_require_t<same_as<I, decltype(std::declval<const iter_difference_t<I>&>()+std::declval<const I&>())>>,
  meta::_require_t<same_as<I, decltype(std::declval<const I&>()-std::declval<const iter_difference_t<I>&>())>>,
  meta::_require_t<same_as<iter_reference_t<I>, decltype(std::declval<const I&>()[std::declval<const iter_difference_t<I>&>()])>>
  >//void_t
  >
  :std::true_type
  {};
};

template<>
struct _iterator_category_concepts<4>
{
  template<class I, class = void>
  struct _impl
    :std::false_type
  {};
  
  template<class I>
  struct _impl<I,std::enable_if_t<same_as<decltype(std::declval<const I&>()),std::add_pointer_t<iter_reference_t<I>>>>>
  :std::true_type
  {};
};

template<class Iter,class T>
CXX17_CONCEPT output_iterator = input_or_output_iterator<Iter>
                                && indirectly_writable<Iter,T>
&& _iterator_category_concepts<1>::template _impl<Iter,T>::value;

template<class Iter>
CXX17_CONCEPT forward_iterator = input_iterator<Iter>
                                 && derived_from<_iter_concept<Iter>, forward_iterator_tag>
&& incrementable<Iter>
  && sentinel_for<Iter,Iter>;

template<class Iter>
CXX17_CONCEPT bidirectional_iterator
  = forward_iterator<Iter>
    && derived_from<_iter_concept<Iter>,bidirectional_iterator_tag>
&& _iterator_category_concepts<2>::_impl<Iter>::value;

template<class Iter>
CXX17_CONCEPT random_access_iterator
  = bidirectional_iterator<Iter>
    && derived_from<_iter_concept<Iter>,random_access_iterator_tag>
&& totally_ordered<Iter>
  && sized_sentinel_for<Iter,Iter>
&& _iterator_category_concepts<3>::_impl<Iter>::value;

template<class Iter>
CXX17_CONCEPT contiguous_iterator
  = random_access_iterator<Iter>
    && derived_from<_iter_concept<Iter>, contiguous_iterator_tag>
&& std::is_lvalue_reference_v<iter_reference_t<Iter>>
  && same_as<iter_value_t<Iter>, meta::remove_cvref_t<iter_reference_t<Iter>>>
&& _iterator_category_concepts<4>::_impl<Iter>::value;
}
#endif //PREVIEW__ITERATOR_CONCEPTS_HPP
