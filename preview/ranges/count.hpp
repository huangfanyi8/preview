#ifndef PREVIEW_RANGES_COUNT_HPP
#define PREVIEW_RANGES_COUNT_HPP

#include "range_primitives.hpp"
#include "input_range.hpp"
#include "invoke.hpp"

namespace preview::ranges::Cpo
{
    struct _count
    {
        template<class I, class S,
            class Proj = identity, class T = projected_value_t<I, Proj>,
            class = std::enable_if_t< indirect_binary_predicate<ranges::equal_to,
                projected<I, Proj>, const T*>&&preview::input_iterator<I>&&sentinel_for<S,I>>>
        constexpr iter_difference_t<I>
        operator()(I first, S last, const T& value, Proj proj = {}) const
        {
            iter_difference_t<I> counter = 0;
            for (; first != last; ++first)
                if (meta::invoke(proj, *first) == value)
                    ++counter;
            return counter;
        }
        
        template<class R, class Proj = ranges::identity,
            class T = projected_value_t<ranges::iterator_t<R>, Proj>,
                class = std::enable_if_t<indirect_binary_predicate<ranges::equal_to,
                    projected<ranges::iterator_t<R>, Proj>,
                    const T*>&&input_range<R>>>
        constexpr ranges::range_difference_t<R>
        operator()(R&& r, const T& value, Proj proj = {}) const
        {
            return (*this)(ranges::begin(r), ranges::end(r), value, std::move(proj));
        }
    };
    
    struct _count_if
    {
        template<class I, class S,
            class Proj = identity,class Pred,
                class = std::enable_if_t<input_iterator<I>
                    &&sentinel_for<S,I>
                    &&indirect_unary_predicate<Pred,projected<I, Proj>>>>
        constexpr iter_difference_t<I>
        operator()(I first, S last, Pred pred, Proj proj = {}) const
        {
            iter_difference_t<I> counter = 0;
            for (; first != last; ++first)
                if (meta::invoke(pred, meta::invoke(proj, *first)))
                    ++counter;
            return counter;
        }
        
        template<class R, class Proj = identity,
            class Pred,
            class = std::enable_if_t<ranges::input_range <R>
                &&indirect_unary_predicate<Pred,projected<ranges::iterator_t<R>, Proj>>>>
        constexpr ranges::range_difference_t<R>
        operator()(R&& r, Pred pred, Proj proj = {}) const
        {
            return (*this)(ranges::begin(r), ranges::end(r),std::move(pred), std::move(proj));
        }
    };
}

namespace preview::ranges
{
    inline constexpr Cpo::_count count;
    inline constexpr Cpo::_count_if  count_if;
}

#endif //PREVIEW_RANGES_COUNT_HPP
