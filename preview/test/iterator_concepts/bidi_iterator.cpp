#include <cstddef>
#include "../../iterator/single/iterator_concepts.hpp"

struct SimpleInputIterator
{
    using difference_type = std::ptrdiff_t;
    using value_type = int;
    
    int operator*() const;
    
    SimpleInputIterator& operator++();
    void operator++(int) { ++*this; }
};

struct SimpleForwardIterator
{
    using difference_type = std::ptrdiff_t;
    using value_type = int;
    
    int operator*() const;
    
    SimpleForwardIterator& operator++();
    
    SimpleForwardIterator operator++(int)
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }
    
    bool operator==(const SimpleForwardIterator&) const;
    bool operator!=(const SimpleForwardIterator&) const;
};

static_assert(preview::forward_iterator<SimpleForwardIterator>);
static_assert(preview::input_iterator<SimpleForwardIterator>);
static_assert(preview::sentinel_for<SimpleForwardIterator,SimpleForwardIterator>);
static_assert(preview::incrementable<SimpleForwardIterator>);
static_assert(preview::derived_from<preview::_iter_concept<SimpleForwardIterator>, preview::forward_iterator_tag>);
static_assert(preview::_details::_is_cpp17_input_iterator<SimpleForwardIterator>);
static_assert(std::is_same_v<std::input_iterator_tag,
    typename preview::iterator_traits<SimpleForwardIterator>::iterator_category >);

static_assert(std::is_same_v<std::random_access_iterator_tag,preview::_iter_concept<SimpleForwardIterator>>);
static_assert(!preview::_is_specialization_v<SimpleForwardIterator>);
static_assert(preview::input_iterator<SimpleInputIterator>);