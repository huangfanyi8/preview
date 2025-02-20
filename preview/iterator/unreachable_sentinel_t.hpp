#ifndef PREVIEW_UNREACHABLE_SENTINEL_T_HPP
#define PREVIEW_UNREACHABLE_SENTINEL_T_HPP

#include "incrementable.hpp"

namespace preview
{
  struct unreachable_sentinel_t
  {
    template<class I,class=meta::_require_t<weakly_incrementable<I>>>
    friend constexpr bool
    operator==(unreachable_sentinel_t, const I&) noexcept
    { return false; }
  };
  
  inline constexpr unreachable_sentinel_t unreachable_sentinel{};
}
#endif //PREVIEW_UNREACHABLE_SENTINEL_T_HPP
