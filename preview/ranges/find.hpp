#ifndef PREVIEW_RANGES_FIND_HPP
#define PREVIEW_RANGES_FIND_HPP

#include "borrowed_iterator_t.hpp"
#include "invoke.hpp"
#include "input_range.hpp"

namespace preview::ranges
{
    namespace Cpo
    {
        using meta::_require_t;
        
        struct _find
        {
            template<class I,class S,
                class Proj = identity,
                class T = projected_value_t<I, Proj>,
                class=std::enable_if_t<input_iterator<I>
            &&sentinel_for<S,I>
            &&indirect_binary_predicate<ranges::equal_to,projected<I, Proj>, const T*>>>
            constexpr I operator()(I first, S last, const T& value, Proj proj = {}) const
            {
                for (; first != last; ++first)
                    if (meta::invoke(proj, *first) == value)
                        return first;
                return first;
            }
            
            template<class R, class T, class Proj = ranges::identity,
                class=std::enable_if_t<input_range<R>
            &&indirect_binary_predicate<ranges::equal_to,
                projected<ranges::iterator_t<R>, Proj>, const T*>>>
            constexpr ranges::borrowed_iterator_t<R>
            operator()(R&& r, const T& value, Proj proj = {}) const
            {
                return (*this)(ranges::begin(r), ranges::end(r), value, std::move(proj));
            }
        };
        
        struct _find_if
        {
            template<class I,class S,class Pred,
                class Proj = identity,
                class=std::enable_if_t<indirect_unary_predicate<Pred,projected<I, Proj>>>>
            constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const
            {
                for (; first != last; ++first)
                    if (meta::invoke(pred,meta::invoke(proj, *first)))
                        return first;
                return first;
            }
            
            template<class R,class Pred,class Proj = identity,
                class=std::enable_if_t<input_range<R>
            &&indirect_unary_predicate<Pred,projected<ranges::iterator_t<R>, Proj>>>>
            constexpr ranges::borrowed_iterator_t<R>
            operator()(R&& r, Pred pred, Proj proj = {}) const
            {
                return (*this)(ranges::begin(r), ranges::end(r), std::move(pred), std::move(proj));
            }
        };
        
        struct _find_if_not
        {
            template<class I,class S,class Pred,
                class Proj = identity,
                class=std::enable_if_t<indirect_unary_predicate<Pred,projected<I, Proj>>>>
            constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const
            {
                for (; first != last; ++first)
                    if (!meta::invoke(pred,meta::invoke(proj, *first)))
                        return first;
                return first;
            }
            
            template<class R,class Pred,class Proj = identity,
                class=std::enable_if_t<input_range<R>
            &&indirect_unary_predicate<Pred,projected<ranges::iterator_t<R>, Proj>>>>
            constexpr ranges::borrowed_iterator_t<R>
            operator()(R&& r, Pred pred, Proj proj = {}) const
            {
                return (*this)(ranges::begin(r), ranges::end(r), std::move(pred), std::move(proj));
            }
        };
    }
    inline constexpr Cpo::_find find;
    inline constexpr Cpo::_find_if find_if;
    inline constexpr Cpo::_find_if_not find_if_not;
}

#endif
