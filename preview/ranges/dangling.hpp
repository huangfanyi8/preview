#ifndef PREVIEW_RANGES_DANGLING_HPP
#define PREVIEW_RANGES_DANGLING_HPP

namespace preview::ranges
{
    struct dangling
    {
        constexpr dangling() noexcept = default;
        template<class...Args>
        constexpr dangling(Args&&...) noexcept { }
    };
}

#endif //PREVIEW_RANGES_DANGLING_HPP
