
#ifndef PREVIEW_RANGES_ADVANCE_HPP
#define PREVIEW_RANGES_ADVANCE_HPP
#include "borrowed_iterator_t.hpp"
#include "invoke.hpp"
#include "input_range.hpp"

namespace preview::ranges::Cpo
{
    using meta::_require_t;
    struct _advance
    {
        template<class I,class=std::enable_if_t<input_or_output_iterator<I>>>
        constexpr void operator()(I& i,iter_difference_t<I> n) const
        {
            if constexpr (random_access_iterator<I>)
                i += n;
            else
            {
                while (n > 0)
                {
                    --n;
                    ++i;
                }
                
                if constexpr (bidirectional_iterator<I>)
                {
                    while (n < 0)
                    {
                        ++n;
                        --i;
                    }
                }
            }
        }
        
        template<class I,class S,
            class=std::enable_if_t<input_or_output_iterator<I>&&sentinel_for<S,I>>>
        constexpr void operator()(I& i, S bound) const
        {
            if constexpr (assignable_from<I&, S>)
                i = std::move(bound);
            else if constexpr (sized_sentinel_for<S, I>)
                (*this)(i, bound - i);
            else
                while (i != bound)
                    ++i;
        }
        
        template<class I,class S,
            class=std::enable_if_t<input_or_output_iterator<I>&&sentinel_for<S,I>>>
        constexpr iter_difference_t<I>
        operator()(I& i, iter_difference_t<I> n, S bound) const
        {
            if constexpr (sized_sentinel_for<S, I>)
            {
                // std::abs is not constexpr until C++23
                auto abs = [](const iter_difference_t<I> x) { return x < 0 ? -x : x; };
                
                if (const auto dist = abs(n) - abs(bound - i); dist < 0)
                {
                    (*this)(i, bound);
                    return -dist;
                }
                
                (*this)(i, n);
                return 0;
            }
            else
            {
                while (n > 0 && i != bound)
                {
                    --n;
                    ++i;
                }
                
                if constexpr (bidirectional_iterator<I>)
                {
                    while (n < 0 && i != bound)
                    {
                        ++n;
                        --i;
                    }
                }
                
                return n;
            }
        }
    };
    
    struct _next final
    {
        template<class It,class= std::enable_if_t<input_or_output_iterator<It>>>
        [[nodiscard]]
        constexpr It operator()(It x) const
        {
            ++x;
            return x;
        }
        
        template<class It,class=std::enable_if_t<input_or_output_iterator<It>>>
        [[nodiscard]]
        constexpr It operator()(It x, iter_difference_t<It> n) const
        {
            _advance{}(x,n);
            return x;
        }
        
        template<class It,class Sent,
            class=std::enable_if_t<input_or_output_iterator<It>>,
            class=std::enable_if_t<sentinel_for<Sent,It>>>
        constexpr It
        operator()(It  it, Sent bound)const
        {
            _advance{}(it, bound);
            return it;
        }
        
        template<class It,class Sent,
            class=std::enable_if_t<input_or_output_iterator<It>>,
            class=std::enable_if_t<sentinel_for<Sent,It>>>
        [[nodiscard]]
        constexpr It
        operator()(It it, iter_difference_t<It> n, Sent bound) const
        {
            _advance{}(it, n, bound);
            return it;
        }
        
        void operator&() const = delete;
    };
    
    struct _prev final
    {
        template<class It,class=meta::_require_t<bidirectional_iterator<It>>>
        [[nodiscard]]
        constexpr It operator()(It it) const
        {
            --it;
            return it;
        }
        
        template<class It,class=_require_t<bidirectional_iterator<It>>>
        [[nodiscard]]
        constexpr It operator()(It iter, iter_difference_t<It> n) const
        {
            _advance{}(iter, -n);
            return iter;
        }
        
        template<class It,class=_require_t<bidirectional_iterator<It>>>
        [[nodiscard]]
        constexpr It operator()(It x, iter_difference_t<It> n, It bound) const
        {
            _advance{}(x, -n, bound);
            return x;
        }
        
        void operator&() const = delete;
    };
}

namespace preview::ranges
{
    inline constexpr auto advance = Cpo::_advance();
    inline constexpr Cpo::_next next;
    inline constexpr Cpo::_prev prev;
}
#endif //PREVIEW_RANGES_ADVANCE_HPP
