
#ifndef PREVIEW__BASE_HPP
#define PREVIEW__BASE_HPP

#include"../meta/compatible.hpp"
#include "iterator"

namespace preview
{
  using std::input_iterator_tag;
  using std::forward_iterator_tag;
  using std::output_iterator_tag;
  using std::bidirectional_iterator_tag;
  using std::random_access_iterator_tag;
  struct contiguous_iterator_tag : public random_access_iterator_tag {};
}

#endif //PREVIEW__BASE_HPP
