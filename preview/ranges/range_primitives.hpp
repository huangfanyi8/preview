
#ifndef PREVIEW_RANGE_PRIMITIVES_HPP
#define PREVIEW_RANGE_PRIMITIVES_HPP

#include "cbegin.hpp"
#include "cend.hpp"
#include "end.hpp"
#include "range.hpp"
#include "size.hpp"

namespace preview::ranges::_details
{
    template<class T, class = void>
    struct _iterator {};
    
    template<class T>
    struct _iterator<T, std::void_t<decltype( ranges::begin(std::declval<T&>()) )>>
    {
        using type = decltype(ranges::begin(std::declval<T&>()));
    };
    
    template< class R ,class = void,class = void>
    struct _const_iterator
    {};
    
    template<class R>
    struct _const_iterator<R,std::enable_if_t<ranges::range<R>>,std::void_t<decltype(ranges::cbegin(std::declval<R&>()))>>
    {using type = decltype(ranges::cbegin(std::declval<R&>()));};
    
    template< class R ,class = void,class = void>
    struct _sentinel
    {};
    
    template<class R>
    struct _sentinel<R,std::enable_if_t<ranges::range<R>>,std::void_t<decltype(ranges::end(std::declval<R&>()))>>
    {using type = decltype(ranges::end(std::declval<R&>()));};
    
    template< class R ,class = void,class = void>
    struct _const_sentinel
    {};
    
    template<class R>
    struct _const_sentinel<R,std::enable_if_t<ranges::range<R>>,std::void_t<decltype(ranges::cend(std::declval<R&>()))>>
    {using type = decltype(ranges::cend(std::declval<R&>()));};
    
    template< class R ,class = void,class = void>
    struct _range_size
    {};
    
    template<class R>
    struct _range_size<R,std::enable_if_t<ranges::range<R>>,std::void_t<decltype(ranges::cend(std::declval<R&>()))>>
    {using type = decltype(ranges::size(std::declval<R&>()));};
}

namespace preview::ranges
{
    template< class T >
    using iterator_t = typename _details::_iterator<T&>::type;
    
    template< class R ,class = std::enable_if_t<ranges::range<R>>>
    using const_iterator_t = typename _details::_const_iterator<R&>::type;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>>>
    using sentinel_t = typename _details::_sentinel<R&>::type;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using const_sentinel_t = typename _details::_const_sentinel<R&>::type;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_size_t = typename _details::_range_size<R>::type;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_difference_t = iter_difference_t<ranges::iterator_t<R>>;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_value_t = iter_value_t<ranges::iterator_t<R>>;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_reference_t = iter_reference_t<ranges::iterator_t<R>>;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_const_reference_t = iter_const_reference_t<ranges::iterator_t<R>>;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_rvalue_reference_t = iter_rvalue_reference_t<ranges::iterator_t<R>>;
    
    template< class R ,class =std::enable_if_t<ranges::range<R>> >
    using range_common_reference_t = iter_common_reference_t<ranges::iterator_t<R>>;
}
#endif //PREVIEW_RANGE_PRIMITIVES_HPP
