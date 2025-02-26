#ifndef PREVIEW_SINGLE_VARIANT_HPP
#define PREVIEW_SINGLE_VARIANT_HPP

#if defined(_MSVC_LANG)
#define STL_LANG _MSVC_LANG
#define  unreachable() __assume(false)
#else
#define STL_LANG __cplusplus
#define  unreachable() __builtin_unreachable()
#endif

#include<utility>
#include <exception>
#include "string"

namespace utility
{
  inline constexpr auto variant_npos = size_t(-1);
  
  template<class...>class variant;
  
  template<class T>struct _identity{using type = T;};
  
  template<std::size_t,class,class = void>
  struct get{};
  
  template<std::size_t  _idx,template<class...>class TL,class F,class...R>
  struct get<_idx,TL<F,R...>>
    :std::conditional_t<_idx==0,_identity<F>,get<_idx-1,TL<R...>>>
  {};
  
  template<class T,class U>
  struct _copy_cv
    :std::conditional_t<std::is_const_v<T>,
    std::conditional_t<std::is_volatile_v<T>,std::add_cv<U>,std::add_const<U>>,
      std::conditional_t<std::is_volatile_v<T>,std::add_volatile<U>,_identity<U>>>
  {};
  
  template<class T,class U>
  using _copy_cv_t = typename _copy_cv<T,U>::type;
  
  template<class T,class U>
  struct _copy_ref
    :std::conditional_t<std::is_lvalue_reference_v<T>,
    std::add_lvalue_reference<U>,
    std::conditional_t<std::is_rvalue_reference_v<T>,
    std::add_rvalue_reference<U>,_identity<U>>>
  {};
  
  template<class T,class U>
  struct _copy_cvref
    :_copy_ref<T,_copy_cv_t<T,U>>
  {};
  
  template<std::size_t _idx,class T>
  using get_t = typename get<_idx,T>::type;
  
  template<class T>
  using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
  
  template<class T,std::size_t  _idx>
  struct get<_idx,T,std::enable_if_t<std::disjunction_v<std::is_const<T>,std::is_volatile<T>>>>
    :_copy_cvref<T,get_t<_idx,remove_cvref_t<T>>>
  {};
  
  template<class...>class _aux{};
  
  static_assert(std::is_same_v<const get_t<0,_aux<int,double>>,const int>);
  static_assert(std::is_same_v<volatile get_t<0,_aux<int,double>>,volatile int>);
  static_assert(std::is_same_v<const volatile get_t<0,_aux<int,double>>,const volatile int>);
}

namespace utility
{
  class bad_variant_access
    : public std::exception
  {
  public:
    bad_variant_access() noexcept = default;
    
    [[nodiscard]] const char* what() const noexcept override
    { return M_reason; }
  
  private:
    explicit bad_variant_access(const char* _reason) noexcept : M_reason(_reason) { }
    
    // Must point to a string with static storage duration:
    const char* M_reason = "bad variant access";
    
    friend void throw_bad_variant_access(const char*);
  };
  
  inline void throw_bad_variant_access(const char*str="bad variant access")
  {throw bad_variant_access{str};}
  
}

//variant_size
namespace utility
{
  namespace _variant
  {
    template<class>
    inline constexpr size_t _variant_size_v = -1;
    
    template<template<class...>class T,class...A>
    inline constexpr size_t _variant_size_v<T<A...>> = sizeof...(A);
  }
  
  template<class T>
  inline constexpr size_t variant_size_v= _variant::_variant_size_v<remove_cvref_t<T>>;
  
  template<class T>
  struct variant_size
    :std::integral_constant<size_t,variant_size_v<T>>
  {};
}

namespace utility::_variant
{
  template<class Ti>
  struct _construct_array
  {Ti x[1];};
  
  //Ti x[1] ={std::forward<T>(t)}
  template<class TI,class T,class =void>
  inline constexpr bool _match_v =  false;
  
  template<class TI,class T>
  inline constexpr bool _match_v<TI,T,std::void_t<decltype(_construct_array<TI>{std::declval<T>()})>>  = true;
  
  template<class Input,class TL>
  struct _variant_choice
  {
  private:
    template<class,class >
    struct _impl
      :_identity<void>
    {};
    
    template<class T,template<class...>class L,class Head,class...Rest>
    struct _impl<T,L<Head,Rest...>>
      :std::conditional_t<std::is_same_v<T,Head>,
        _identity<Head>,
        std::conditional_t<_match_v<Head,T>,
          _identity<Head>,
          _impl<T,L<Rest...>>>>
    {};
  
  public:
    using type= typename _impl<Input,TL>::type;
  };
  
  template<class Input,class TL>
  using _variant_choice_t=typename _variant_choice<Input,TL>::type;
  
  static_assert(std::is_same_v<_variant_choice_t<const char*,_aux<const char*,std::string>>,const char*>);
  
  static_assert(std::is_same_v<_variant_choice_t< const char*,_aux<std::string, bool, char>>, std::string>);
  static_assert(std::is_same_v<_variant_choice_t<int,_aux<float, long, double>>, long>);
}

//Make all index table
namespace  utility::_variant
{
  template<class,std::size_t...>struct _gen_table_base;
  //_aux<>
  template<std::size_t _new>
  struct _gen_table_base<_aux<>,_new>
  {
  private:
    template<class>struct _impl{};
    
    template<std::size_t..._val>
    struct _impl<std::index_sequence<_val...>>
      :_identity<_aux<std::index_sequence<_val>...>>
    {};
  
  public:
    using type = typename _impl<std::make_index_sequence<_new>>::type;
  };
  
  template<class...Index,std::size_t _new>
  struct _gen_table_base<_aux<Index...>,_new>
{
  private:
  using  _sequence = std::make_index_sequence<_new>;
  
  template<class,class>
  struct _base{};
  
  template<std::size_t..._old,std::size_t ..._new_val>
  struct _base<std::index_sequence<_old...>,std::index_sequence<_new_val...>>
    :_identity<_aux<std::index_sequence<_old...,_new_val>...>>
  {};

  template<class...>
  struct _merge{};
  
  template<class...Val>
  struct _merge<_aux<Val...>>
    :_identity<_aux<Val...>>
  {};

  template<class..._1,class..._2>
  struct _merge<_aux<_1...>,_aux<_2...>>
    :_identity<_aux<_1...,_2...>>
  {};

  template<class A,class B,class...R>
  struct _merge<A,B,R...>
    :_merge<typename _merge<A,B>::type,R...>
  {};
  public:
  using type = typename _merge<typename _base<Index,_sequence>::type...>::type;
  };
  
  template<class Old,std::size_t..._v>
  using  _gen_table_base_t = typename _gen_table_base<Old,_v...>::type;
  
  template<class...Index,std::size_t _first,std::size_t..._rest>
  struct _gen_table_base<_aux<Index...>,_first,_rest...>
  :_gen_table_base<_gen_table_base_t<_aux<Index...>,_first>,_rest...>
  {};
  
  template<std::size_t..._size>
  using _gen_table_t =_gen_table_base_t<_aux<>,_size...>;
}

//_product_v
namespace  utility::_variant
{
    template<class...Variants>
    inline constexpr auto _product_v = (variant_size_v<Variants>*...*1);
}

//_radix-table
namespace  utility::_variant
{
    template<class O,class...Variants>
    struct _radix_table
        :_identity<O>
    {};
    
    template<size_t..._index,class Head,class...Rest>
    struct _radix_table<std::index_sequence<_index...>,Head,Rest...>
        :std::conditional_t<bool(sizeof...(Rest)),
        _radix_table<std::index_sequence<_index...,_product_v<Rest...>>,Rest...>,
        _identity<std::index_sequence<_index...,1>>
    >
    {};
    
    template<class...Variants>
    using _radix_table_t = typename _radix_table<std::index_sequence<>,Variants...>::type;
    
    static_assert(std::is_same_v<_radix_table_t<_aux<int,double>,_aux<int,double,char>,_aux<int,double>>,
    std::index_sequence<6,2,1>>);
}

//_unique
namespace  utility::_variant
{
  template<class>struct _make_empty;
  
  template<template<class...>class TL,class...A>
  struct _make_empty<TL<A...>>
  {using type = TL<>;};
  
  template<class T,class E=typename _make_empty<T>::type>
  struct _is_unique
    :std::true_type
  {};
  
  template<template<class ...>class TL,class F,class...R,class...A>
  struct _is_unique<TL<F,R...>,TL<A...>>
    :std::conditional_t<(std::is_same_v<F,A>||...||false),std::false_type ,_is_unique<TL<R...>,TL<A...,F>>>
  {};
  
  template<class TL>
  inline constexpr bool _is_unique_v = _is_unique<TL>::value;
}

//_index
namespace  utility::_variant
{
  using _variant_error =  std::integral_constant<size_t ,variant_npos>;
  
  template<size_t _count,template<class,class>class Tr,class T,class TL>
  struct _variant_index
    :_variant_error
  {};
  
  template<size_t _count,template<class,class>class Tr,class T,template<class...>class TL,class F,class...R>
  struct _variant_index<_count,Tr,T,TL<F,R...>>
    :std::conditional_t<Tr<F,T>::value,std::integral_constant<size_t,_count>,_variant_index<_count+1,Tr,T,TL<R...>>>
  {};
  
  template<template<class,class>class Tr,class T,class TL>
  inline constexpr auto _index_v = _variant_index<0,Tr,remove_cvref_t<T>,remove_cvref_t<TL>>::value;
  
  template<class T,class TL>
  inline constexpr auto _unique_index_v = _index_v<std::is_same,T,TL>;
  
  template<class T,class TL>
  inline constexpr  bool _valid_v = _unique_index_v<T,TL>!=variant_npos;
}
//_variant_storage
namespace utility::_variant
{
    template<size_t...>struct _max{};
    
    template<size_t _v>
    struct _max<_v>
      :std::integral_constant<size_t ,_v>
    {};
    
    template<size_t _a,size_t _b>
    struct _max<_a,_b>
      :std::integral_constant<size_t ,(_a>_b)?_a:_b>
    {};
    
    template<size_t _a,size_t _b,size_t _c,size_t..._r>
    struct _max<_a,_b,_c,_r...>
      :_max<(_a>_b)?_a:_b,_c,_r...>
    {};
    
    template<class...Types>
    struct _variant_storage
    {
      using _self = _variant_storage<Types...>;
      using _destroy_func=void(const void*);
      
      alignas(Types...) char _buffer[_variant::_max<sizeof (Types)...>::value]{};
      size_t _index = variant_npos;
      
      static constexpr _destroy_func* _destroy_table[sizeof...(Types)]=
        {
          [](const void*addr)
          {
            auto v=static_cast<_variant_storage*>(const_cast<void*>(addr));
            static_cast<Types*>(static_cast<void*>(&v->_buffer))->~Types();
          }...
        };
      
      constexpr _variant_storage()noexcept=default;
      
      template<std::size_t _idx,class...Args,class = std::enable_if_t<(_idx< sizeof...(Types))>>
      constexpr explicit _variant_storage(std::in_place_index_t<_idx>,Args&&...args)
      {_do_construct<_idx>(std::forward<Args>(args)...);}
      
      template<std::size_t _idx,class U,class...Args,class = std::enable_if_t<(_idx< sizeof...(Types))>>
      constexpr explicit _variant_storage(std::in_place_index_t<_idx>,std::initializer_list<U> _list,Args&&...args)
      {_do_construct<_idx>(_list,std::forward<Args>(args)...);}
      
      template<class T,class...Args,
        class = std::enable_if_t<_variant::_valid_v<T, _variant_storage>>>
      constexpr explicit _variant_storage(std::in_place_type_t<T>,Args&&...args)
      {
        this->_index=_variant::_unique_index_v<T,_self>;
        ::new(static_cast<void*>(&_buffer))T(std::forward<Args>(args)...);
      }
      
      template<class T,class U,class...Args,class = std::enable_if_t<_variant::_valid_v<T, _variant_storage>>>
      constexpr explicit _variant_storage(std::in_place_type_t<T>,std::initializer_list<U> _list,Args&&...args)
      {
        this->_index=_variant::_unique_index_v<T,_self>;
        ::new(static_cast<void*>(&_buffer))T(_list,std::forward<Args>(args)...);
      }
      
      [[nodiscard]] constexpr size_t index()const{return _index;}
      
      constexpr void reset()noexcept
      {
        if(this->_index!=variant_npos)
          _destroy_table[this->_index](static_cast<const void *>(this));
        this->_index=variant_npos;
      }
      
      template <class T, class... Args,
        class = std::enable_if_t<std::is_constructible<T,Args...>::value&&_variant::_valid_v<T,_self>>>
      constexpr T&emplace(Args&&...args)
      {
        reset();
        this->_index=this->_index=_variant::_unique_index_v<T,_self>;;
        ::new(static_cast<void*>(&_buffer))T(std::forward<Args>(args)...);
        return this->template get<T>();
      }
      
      template <class T, class U, class... Args,
        class=std::enable_if_t<std::is_constructible<T,std::initializer_list<U>&,Args...>::value&&_variant::_valid_v<T,_self>>>
      constexpr T&emplace(std::initializer_list<U> list, Args&&...args)
      {
        reset();
        this->_index=this->_index=_variant::_unique_index_v<T,_self>;;
        ::new(static_cast<void*>(&_buffer))T(list,std::forward<Args>(args)...);
        return this->template get<T>();
      }
      
      template <size_t I,
        class T = get_t<I,_self>,class... Args,
        class = std::enable_if_t<std::is_constructible<T,Args...>::value&& _variant::_valid_v<T,_self>>>
      constexpr T&emplace(Args&&...args)
      {return this->emplace<T>(std::forward<Args>(args)...);}
      
      template <size_t I,
        class U,class T = get_t<I,_self>,class... Args,
        class = std::enable_if_t<std::is_constructible<T,std::initializer_list<U>&,Args...>::value&&_variant::_valid_v<T,_self>>>
      constexpr T&emplace(std::initializer_list<U> list, Args&&...args)
      {return this->emplace<T>(list,std::forward<Args>(args)...);}
      
      ~_variant_storage(){reset();}
      
      #define MAKE_GET(RETURN,POSTFIX,A1,A2,...) \
      template <class T,class=std::enable_if_t<_variant::_valid_v<T,_self>>>\
      constexpr RETURN get()POSTFIX             \
      {                                          \
        if (this->_index==variant_npos)\
          throw bad_variant_access();\
        return __VA_ARGS__(*static_cast<A1>(static_cast<A2>(&_buffer))); \
      }\
      template <size_t _index,class T= get_t<_index,_self>, \
        class=std::enable_if_t<_variant::_valid_v<T,_self>>>\
      constexpr  RETURN get()POSTFIX             \
      {                                          \
        if (this->_index==variant_npos)\
          throw bad_variant_access();\
        return __VA_ARGS__(*static_cast<A1>(static_cast<A2>(&_buffer))); \
      }
      
      MAKE_GET(T&,&,T*,void*)
      MAKE_GET(const T&,const&,const T*,const void*)
      MAKE_GET(T&&,&&,T*,void*,std::move)
      MAKE_GET(const T&&,const&&,const T*,const void*,std::move)

#undef MAKE_GET

//Do construct but no check
      template<size_t _index,class...Args>
      constexpr void _do_construct(Args&&...args)
      {
        using T = get_t<_index,_variant_storage>;
        this->_index=_variant::_unique_index_v<T,_self>;
        ::new(static_cast<void*>(&_buffer))T(std::forward<Args>(args)...);
      }
    };
}

//visit
namespace utility::_variant
{
  template<size_t..._v>
  inline constexpr auto _size_sequence = std::index_sequence<_v...>{};
  
  template<size_t _v>
  inline constexpr auto _size_constant = std::integral_constant<size_t ,_v>{};
  
  template<class...T>
  inline constexpr auto _aux_constant = _aux<T...>{};
  
  #define VISIT_CASE4(_n, _case) \
    _case(_n);                 \
    _case(_n + 1);             \
    _case(_n + 2);             \
    _case(_n + 3)
  
  #define VISIT_CASE16(_n, _case) \
    VISIT_CASE4(_n, _case);     \
    VISIT_CASE4(_n + 4, _case); \
    VISIT_CASE4(_n + 8, _case); \
    VISIT_CASE4(_n + 12,_case)
  
  #define VISIT_CASE64(_n, _case)   \
    VISIT_CASE16(_n, _case);      \
    VISIT_CASE16(_n + 16, _case); \
    VISIT_CASE16(_n + 32,_case); \
    VISIT_CASE16(_n + 48, _case)

#define VISIT_CASE256(_n, _case)   \
    VISIT_CASE64(_n, _case);       \
    VISIT_CASE64(_n + 64, _case);  \
    VISIT_CASE64(_n + 128, _case); \
    VISIT_CASE64(_n + 192,_case)
  
  template<class...Variants,size_t..._indices>
  constexpr size_t _count_index(std::index_sequence<_indices...>,const Variants&...vars)
  {return ((_indices*vars.index())+...+0);}

  struct _variant_idx_cookie
  {
    template <class R,size_t..._indices,class Visitor,class...Variants>
    static constexpr R _invoke(std::index_sequence<_indices...>,Visitor&&visitor,Variants&&...vs)
    {return std::forward<Visitor>(visitor)(std::forward<Variants>(vs).template get<_indices>()...,_size_constant<_indices>...);}
  };
  
  struct _variant_cookie
  {
    template <class R,size_t..._indices,class Visitor,class...Variants>
    static constexpr R _invoke(std::index_sequence<_indices...>,Visitor&&visitor,Variants&&...vs)
    {return std::forward<Visitor>(visitor)(std::forward<Variants>(vs).template get<_indices>()...);}
  };
  
  template <class Tag,size_t _begin,class R,class Visitor,class...Variants>
  constexpr R _do_visit(Visitor&&visitor,Variants&&...vs)
  {
    using _table = _radix_table_t<_aux<remove_cvref_t<Variants>...>>;
    constexpr size_t size = _product_v<remove_cvref_t<Variants>...>;
    const size_t _index = _count_index(_table{}, vs...);
    using _array = _gen_table_t<variant_size<remove_cvref_t<Variants>>::value...>;
    
    #define _case(_n) \
    case _begin+_n:   \
    {\
      if constexpr(_begin+_n< size)\
      {\
        using _sequence = get_t<_begin+_n,_array>;\
        if constexpr(std::is_same_v<Tag,_variant_idx_cookie>)\
          return _variant_idx_cookie::_invoke<R>(_sequence {},std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);\
        else\
          return _variant_cookie::_invoke<R>(_sequence {},std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);\
      }\
      else\
        unreachable();\
    }
    
    switch (_index)
    {
      VISIT_CASE256(0, _case)
      default:
      {
        if constexpr(_begin+256<size)
          return  _variant::_do_visit<Tag,_begin+256,R>(std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);
        else
          unreachable();
      }
    }//switch
  }
  
  template <class R,class Visitor,class...Variants>
  constexpr R _visit_idx(Visitor&&visitor,Variants&&...vs)
  {return _variant::_do_visit<_variant_idx_cookie,0,R>(std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);}
  
  template<class...Types>constexpr variant<Types...>&             _as_variant_impl(variant<Types...>&);
  template<class...Types>constexpr const variant<Types...>&    _as_variant_impl(variant<Types...>const&);
  template<class...Types>constexpr variant<Types...>&&                _as_variant_impl(variant<Types...>&&);
  template<class...Types>constexpr const variant<Types...>&&  _as_variant_impl(const variant<Types...>&&);
  
  template <class Variant>
  using _as_variant = decltype(_as_variant_impl(std::declval<Variant>()));
  
  template <class Callable, class... Types>
  using _visit_result_t = decltype(std::declval<Callable>()(std::declval<Types>().template get<0>()...));
}

//_variant_base
namespace utility::_variant
{
    template<class ...Types>
    struct _variant_base
    :_variant::_variant_storage<Types...>
    {
    private:
        using Base = _variant::_variant_storage<Types...>;
        public:
        using Base::Base;
        using Base::get;
        using Base::emplace;
        using Base::reset;
        using Base::index;
        private:
        using Base::_index;
        using Base::_do_construct;
    public:
        constexpr _variant_base()noexcept = default;
        
        constexpr _variant_base(const _variant_base& other)
        {
            _variant::_visit_idx<void>([this](auto&&value,auto _index)mutable{
                if constexpr(_index!=variant_npos)
                    this->template emplace<_index>(value);
            },other);
            this->_index=other._index;
        }
        
        constexpr _variant_base(_variant_base&& other)noexcept
        {
            _variant::_visit_idx<void>([this](auto&&value,auto _index)mutable{
                if constexpr(_index!=variant_npos)
                    this->template emplace<_index>(std::forward<decltype(value)>(value));
                },std::move(other));
            this->_index=other._index;
        }
        
        constexpr _variant_base&operator=(const _variant_base& other)
        {
            _visit_idx<void>([this](auto&&value,auto _index)mutable{
                                    constexpr auto _j = _index;
                                    if constexpr (_j==variant_npos)
                                        this->reset();
                                    else
                                        this->template emplace<_j>(value);
                            },other);
            this->_index=other._index;
            return *this;
        }
        
        constexpr _variant_base&operator=(_variant_base&& other)noexcept
        {
            _visit_idx<void>([this](auto&&value,auto _index)mutable{
                if constexpr (_index==variant_npos)
                    this->reset();
                if constexpr(_index!=variant_npos)
                    this->template emplace<_index>(std::forward<decltype(value)>(value));
            },std::move(other));
            this->_index=other._index;
            return *this;
        }
        
        ~_variant_base()noexcept = default;
    };
}

//variant
namespace utility
{
    template <class R,class Visitor,class...Variants>
    constexpr R visit(Visitor&&visitor,Variants&&...vs)
    {return _variant::_do_visit<_variant::_variant_cookie,0,R>(std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);}
    
    template <class Visitor,class...Variants>
    constexpr _variant::_visit_result_t<Visitor, _variant::_as_variant<Variants>...> visit(Visitor&&visitor,Variants&&...vs)
    {
    using R = _variant::_visit_result_t<Visitor, _variant::_as_variant<Variants>...>;
    return utility::visit<R>(std::forward<Visitor>(visitor), std::forward<Variants>(vs)...);
    }
    
    template<class...Types>
    class variant
        : private _variant::_variant_base<Types...>
    {
        //static_assert
        static_assert((sizeof...(Types)>0),"Parameter package size cannot be 0");
        static_assert((!std::is_void_v<Types>&&...&&false),"Void type are not allowed in variant");
        static_assert((!std::is_reference_v<Types>&&...&&false),"Reference type are not allowed in variant");
        static_assert(_variant::_is_unique_v<variant>,"Duplicate elements are not allowed in the variant");
    private:
        using Base = _variant::_variant_base<variant<Types...>>;
        template<class>struct _delete : std::true_type{};
        template<class T>struct _delete<std::in_place_type_t<T>>: std::false_type{};
        template<class...T>struct _delete<variant<T...>>: std::false_type{};
        template<size_t _i>struct _delete<std::in_place_index_t<_i>>: std::false_type{};
    public:
        constexpr variant() noexcept = default;
        constexpr variant(const variant &) noexcept = default;
        constexpr variant(variant &&) noexcept = default;
        ~variant() noexcept = default;
        variant &operator=(const variant &) noexcept = default;
        variant &operator=(variant &&) noexcept = default;
    public:
        using Base::get;
        using Base::emplace;
        using Base::index;
        using Base::Base;
    public:
        template<class T,class Ti= _variant::_variant_choice_t<T, variant>,
            class=std::enable_if_t<std::is_constructible<Ti, T>::value && _delete<remove_cvref_t<T>>::value>>
        constexpr variant(T &&value) noexcept
            :variant{std::in_place_type<Ti>, static_cast<T>(value)}
        {}
    
        template<class ... UTypes, class = std::enable_if_t<!std::is_same_v<variant<Types...>,variant<UTypes...>>>>
        constexpr variant()
        {}
    public:
        [[nodiscard]] constexpr bool valueless_by_exception() const noexcept { return this->_index == variant_npos; }
        
        template<class Type>
        [[nodiscard]] constexpr bool is() const noexcept
        {return this->_index == _variant::_unique_index_v<Type, variant>;}
        
        template<size_t _index>
        [[nodiscard]] constexpr bool is() const noexcept { return _index == this->_index; }
        
        void swap(variant<Types...>&other)
        noexcept(((std::is_nothrow_move_constructible_v<Types> &&std::is_nothrow_swappable_v<Types>) && ...))
        {
            _variant::_visit_idx(
                [this, &other](auto&& _rhs_mem, auto _rhs_index) mutable
                {
                    if constexpr (_rhs_index != variant_npos)
                    {
                        //other与this都有值
                        if (this->index() == _rhs_index)
                        {
                            auto& _this_mem = this->template get<_rhs_index>();
                            using std::swap;
                            swap(_this_mem, _rhs_mem);
                        }
                        else
                        {
                            if (!this->valueless_by_exception())
                            {
                                auto &tmp(std::forward<decltype(_rhs_mem)>(_rhs_mem));
                                other = std::move(*this);
                                this->_M_destructive_move(_rhs_index,std::move(&tmp));
                            }
                            else
                            {
                                this->_M_destructive_move(_rhs_index,std::move(_rhs_mem));
                                other.reset();
                            }
                        }
                    }
                    else
                    {
                        if (!this->valueless_by_exception())
                        {
                            other = std::move(*this);
                            this->reset();
                        }
                    }
                }, other);
        }
    public:
        friend constexpr bool operator==(const variant &left, const variant &right) noexcept
        {
            if(left.valueless_by_exception()&&right.valueless_by_exception())
                return true;
            else if (left.index() == right.index())
                return visit<bool>([=](auto &&_left, auto &&_right) { return _left == _right; }, left, right);
            else
                return false;
        }
        
        friend constexpr bool operator!=(const variant &left, const variant &right) noexcept
        {
            if (left.index() == right.index())
                return visit<bool>([=](auto &&_left, auto &&_right) { return _left != _right; }, left, right);
            else
                return false;
        }
        
        friend constexpr bool operator>=(const variant &left, const variant &right) noexcept
        {
            if(left.valueless_by_exception()&&right.valueless_by_exception())
                return true;
            else if (left.index() == right.index())
                return visit<bool>([=](auto &&_left, auto &&_right) { return _left >= _right; }, left, right);
            else
                return false;
        }
    };
}
#undef unreachable
#undef VISIT_CASE256
#undef VISIT_CASE1
#undef VISIT_CASE4
#undef VISIT_CASE16
#undef VISIT_CASE64
#endif //PREVIEW_SINGLE_VARIANT_HPP
