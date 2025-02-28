#ifndef PREVIEW_DISTANCE_HPP
#define PREVIEW_DISTANCE_HPP

#include "range_primitives.hpp"
#include "sized_range.hpp"

namespace preview::ranges::Cpo
{
    struct _distance final
    {
        template<class It,class Sent,
            class= meta::_require_t<input_or_output_iterator<It>,!sized_sentinel_for<Sent,It>,sentinel_for<Sent,It>>>
        constexpr iter_difference_t<It>operator()[[nodiscard]](It first,Sent last) const
        {
            iter_difference_t<It> n = 0;
            while (first != last)
            {
                ++first;
                ++n;
            }
            return n;
        }
        
        template<class It,class Sent,
            class= meta::_require_t<input_or_output_iterator<It>>>
        [[nodiscard]]
        constexpr iter_difference_t<It>
        operator()(const It& first, const Sent& last) const
        {
            return last - first;
        }
        
        template<class Range,class= meta::_require_t<range<Range>>>
        [[nodiscard]]
        constexpr range_difference_t<Range>
        operator()(Range&& r) const
        {
            if constexpr (sized_range<Range>)
                return static_cast<range_difference_t<Range>>(ranges::size(r));
            else
                return (*this)(ranges::begin(r),ranges::end(r));
        }
        
        void operator&() const = delete;
    };
}
namespace preview::ranges
{
    inline constexpr auto distance = Cpo::_distance{};
}
#endif //PREVIEW_DISTANCE_HPP
