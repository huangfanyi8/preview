
#ifndef PREVIEW__ITER_CONCEPTS_HPP
#define PREVIEW__ITER_CONCEPTS_HPP

#include"iterator_traits.hpp"
#include "iterator_base_types.hpp"

///_iter_concepts
namespace preview
{
  namespace _details
  {
    template<int tag,class Iter,typename =void>
    struct _iter_concept_impl
      :_iter_concept_impl<tag+1,Iter>
    {};
    
    template<typename _Iter>
    struct _iter_concept_impl<1,_Iter,std::void_t< typename iterator_traits<_Iter>::iterator_concept>>
    { using type = typename iterator_traits<_Iter>::iterator_concept; };
    
    template<class Iter>
    struct _iter_concept_impl<2,Iter,std::void_t< typename iterator_traits<Iter>::iterator_category>>
    { using type = typename iterator_traits<Iter>::iterator_category; };
    
    template<class Iter>
    struct _iter_concept_impl<3,Iter,std::enable_if_t<!_is_specialization_v<Iter>>>
    { using type = random_access_iterator_tag; };
    
    template<typename Iter>
    struct _iter_concept_impl<4,Iter>
    { };
    
  }
  
  // ITER_CONCEPT
  template<class Iter>
  using _iter_concept = typename _details::_iter_concept_impl<1,Iter>::type;
}
#endif //PREVIEW__ITER_CONCEPTS_HPP
