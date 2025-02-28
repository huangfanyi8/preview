
/*C++20 std::ranges::iota algorithm*/

#ifndef PREVIEW_RANGES_IOTA_HPP
#define PREVIEW_RANGES_IOTA_HPP

#include "../iterator/iterator_concepts.hpp"
#include "_return_type.hpp"
#include "output_range.hpp"
#include "borrowed_iterator_t.hpp"

namespace preview::ranges::Cpo
{
    struct _iota
    {
        template<class S,class O,class T,
            class = meta::_require_t<input_or_output_iterator<O>,
                sentinel_for<S,O> ,
                indirectly_writable<O, const T&>,
                weakly_incrementable <T>>>
        constexpr out_value_result<O, T> operator()(O first, S last, T value) const
        {
            while (first != last)
            {
                *first = as_const(value);
                ++first;
                ++value;
            }
            return {std::move(first), std::move(value)};
        }
        
        template<class T,class R,
            class = meta::_require_t<weakly_incrementable <T>, ranges::output_range<R,const T&> >>
        constexpr out_value_result<borrowed_iterator_t<R>, T>
        operator()(R&& r, T value) const
        {
            return (*this)(ranges::begin(r), ranges::end(r), std::move(value));
        }
    };
}

namespace preview::ranges{
    inline constexpr Cpo::_iota iota;
}

#endif //PREVIEW_RANGES_IOTA_HPP
