#ifndef PREVIEW_RANGES_ITER_MOVE_HPP
#define PREVIEW_RANGES_ITER_MOVE_HPP

#include "../iterator/iter_reference_t.hpp"

///ranges::iter_move
namespace preview
{
  namespace ranges::Cpo
  {
    void iter_move()=delete;
    
    template<class T,class=void>
    struct _adl_move
      :std::false_type
    {};
    
    template<class T>
    struct _adl_move<T,std::void_t<decltype(iter_move(std::declval<T>()))>>
    :std::bool_constant<meta::_class_or_enum_type<T>>
  {};
  
  struct _iter_move
  {
  private:
    template<class T,class=void>
    struct _result
    { using type = iter_reference_t<T>; };
    
    template<typename T>
    struct _result<T,std::enable_if_t<_adl_move<T>::value>>
    { using type = decltype(iter_move(std::declval<T>())); };
    
    template<typename T>
    struct _result<T,std::enable_if_t<!_adl_move<T>::value
                     && std::is_lvalue_reference_v<iter_reference_t<T>>>>
    { using type = std::remove_reference_t<iter_reference_t<T>>&&; };
    
    template<class T>
    static constexpr bool S_noexcept()
    {
      if constexpr (_adl_move<T>::value)
        return noexcept(iter_move(std::declval<T>()));
      else
        return noexcept(*std::declval<T>());
    }
  
  public:
    
    template<class T,class=std::enable_if_t<meta::_dereference<T>::value>>
    using _type = typename _result<T>::type;
    
    template<class T,class=std::enable_if_t<meta::_dereference<T>::value>>
    [[nodiscard]]
    constexpr decltype(auto)
    operator()(T&& e) const noexcept(S_noexcept<T>())
    {
      if constexpr (_adl_move<T>::value)
        return iter_move(static_cast<T&&>(e));
      else if constexpr (std::is_lvalue_reference_v<decltype(*std::declval<T&>())>)
        return static_cast<_type<T>>(*e);
      else
        return *e;
    }
  };
}

namespace ranges
{
  inline constexpr auto iter_move=Cpo::_iter_move{};
}
}

#endif //PREVIEW_RANGES_ITER_MOVE_HPP
