/*这个文件是关于标准库中type-traits的扩展
 * 包含
 * 1.使用c++17实现c++2x的内容
 * 2.对原标准库中内容的增强实现
 * 3.其他元函数的实现
 * */

#ifndef PREVIEW_META_COMPATIBLE_HPP
#define PREVIEW_META_COMPATIBLE_HPP

#define STL_CXX_14 201402L
#define STL_CXX_17 201703L
#define STL_CXX_20 202002L

#if defined(_MSVC_LANG)
#define STL_LANG _MSVC_LANG
#else
#define STL_LANG __cplusplus
#endif

#define HAS_CXX(VERSION)\
(STL_LANG >= VERSION)

#define STL_14 HAS_CXX(STL_CXX_14)
#define STL_17 HAS_CXX(STL_CXX_17)
#define STL_20 HAS_CXX(STL_CXX_20)

#if STL_17
#define INLINE inline
#else
#define INLINE
#endif

#include<type_traits>

namespace meta
{
  //通用类型，未定乂类型
  class undefined{};
  
  template<class T>struct type_identity{using type=T;};
  
  //bool A B
  //bool A bool B C
  //bool A bool B bool C D
  template<class Bool,class If,class Else,class ...>
  struct conditional
  {};
  
  template<class Bool,class A,class B>
  struct conditional<Bool,A,B>
    :std::conditional<bool(Bool::value),A,B>
  {};
  
  template<class Bool1,class If,class Bool2,class ElseIf1,class... Rest>
  struct conditional<Bool1,If,Bool2,ElseIf1, Rest...>
    :std::conditional<bool(Bool1::value),If,typename conditional<Bool2,ElseIf1,Rest...>::type>
  {};
  
  template<class Type,template<class>class...Pred>
  struct map
    :type_identity<Type>
  {};
  
  template<class Type,template<class>class Pred>
  struct map<Type,Pred>
    :Pred<Type>
  {};
  
  template<class Type,
    template<class>class HeadP,
    template<class>class ...RestP>
  struct map<Type,HeadP,RestP...>
    :map<typename HeadP<Type>::type,RestP...>
  {};
  
  template<class Type,template<class >class...Pred>
  using map_t=typename map<Type,Pred...>::type;
  
  //Advance std::condition
  template<class Bool,class If,class Else,class ... Rest>
  using conditional_t = typename conditional<Bool,If,Else,Rest...>::type;
  
  //Copy const attribute
  template<class From,class To>
  using copy_const_t = std::conditional_t<std::is_const<From>::value, std::add_const_t<To>, To>;
  
  //Copy volatile attribute
  template<class From,class To>
  using copy_volatile_t = std::conditional_t<std::is_volatile<From>::value, std::add_volatile_t<To>, To>;
  
  //Copy vc attribute
  template<class From,class To>
  using copy_cv_t = copy_const_t<From, copy_volatile_t<From, To>>;
  
  //c++2x std::remove_cvref
  template<class T>
  using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;
  
  //c++2x std::remove_cvref_t
  template<class Type>
  using remove_cvref_t = typename remove_cvref<Type>::type;
  
  //Copy reference attribute
  template<class From,class To>
  using copy_reference_t = conditional_t<
    std::is_rvalue_reference<From>, std::add_rvalue_reference_t<To>,
    std::is_lvalue_reference<From>, std::add_lvalue_reference_t<To>,
    To>;
  
  //Copy cvref attribute
  template<class From,class To>
  using copy_cvref_t = copy_reference_t<From, copy_cv_t<From, To>>;
  //Copy cvref attribute
  template<class From,class To>
  using copy_cvref = type_identity<copy_reference_t<From, copy_cv_t<From, To>>>;
  
  template<class T>
  using cref_t = const std::remove_reference_t<T>&;
  
  template<bool..._v>
  using _require_t = std::enable_if_t<(_v&&...&&true)>;
  
  template<bool..._v>
  using _require_or_t = std::enable_if_t<(_v||...||false)>;
  
  template <class T>
  CXX17_CONCEPT _class_or_enum_type
    = std::is_class_v<meta::remove_cvref_t<T>>
      || std::is_enum_v<meta::remove_cvref_t<T>>
      || std::is_union_v<meta::remove_cvref_t<T>>;
}

namespace meta
{
    //CPO class
    template<class T,class U,
      template<class> class TQ, template<class> class UQ>
    struct basic_common_reference
    { };
    
    namespace _common_reference_detail
    {
      enum _reference_t
      {
        _lvalue_reference,
        _rvalue_reference,
        _none
      };
      
      template<class T>
      INLINE constexpr
      _reference_t _reference_cast = std::is_lvalue_reference<T>::value
          ? _lvalue_reference
          : std::is_rvalue_reference<T>::value
          ? _rvalue_reference
          : _none;
      
      
      template<class T,_reference_t v=_reference_cast<T>>
      struct xref
      {
        template<class U>
        using type=std::conditional_t<v == _none,copy_cv_t<T,U>,
        std::conditional_t<v == _lvalue_reference,
          copy_cv_t<T,U>&,
        copy_cv_t<T,U>&&>>;
      };
      
      template<typename Tp1, typename Tp2>
      using basic_common_ref
        = typename basic_common_reference<remove_cvref_t<Tp1>,
      remove_cvref_t<Tp2>,
      xref<Tp1>::template type,
      xref<Tp2>::template type>::type;
      
      template<class X,class Y,class=void>
      struct cond_res
        :type_identity<undefined>
      {};
      
      template<class X,class Y>
      struct cond_res<X,Y,::std::void_t<decltype(false ? ::std::declval<X(&)()>()(): ::std::declval<Y(&)()>()())>>
        :type_identity<decltype(false ? std::declval<X(&)()>()() : std::declval<Y(&)()>()())>
      {};
      
      template<class X,class Y>
      using cond_res_t=typename cond_res<X,Y>::type;
      
      template<class X,class Y>
      using cond_res_cvref_t
        = cond_res_t<copy_cv_t<X,Y>&,copy_cv_t<Y,X>&>;
      
      template<class A,class B>
      struct SimpleCommonReference
      {
      private:
        template<class L,class R,
          _reference_t=_reference_cast<L>,
        _reference_t=_reference_cast<R>>
        struct Impl
          :type_identity<undefined>
        {};
        
        template<class L,class R>
        struct Impl<L,R,_lvalue_reference,_lvalue_reference>
        {
        private:
          using X=map_t<L,std::remove_reference>;
          using Y=map_t<R,std::remove_reference>;
        private:
          using _type=cond_res_cvref_t<X,Y>;

          constexpr static bool _exist
            =std::is_reference<_type>::value&&!std::is_same<remove_cvref_t<_type>,undefined>::value;
        public:
          using type=std::conditional_t<_exist,_type,undefined>;
        };
        
        template<class L,class R>
        struct Impl<L,R,_rvalue_reference,_rvalue_reference>
        {
        private:
          using X=map_t<L,std::remove_reference>;
          using Y=map_t<R,std::remove_reference>;
        
        private:
          using _type=typename Impl<X&,Y&>::type;
          using C=map_t<_type,std::remove_reference>;
        public:
          using type=std::conditional_t<
            std::conjunction_v<std::is_convertible<L,C>,
            std::is_convertible<R,C>>,C,undefined>;
        };
        
        template<class L,class R>
        struct  Impl<L,R,_rvalue_reference,_lvalue_reference>
        {
        private:
          using X=map_t<L,std::remove_reference>;
          using Y=map_t<R,std::remove_reference>;
        
        private:
          using D = typename Impl<const X&, Y&>::type;
        public:
          using type=std::conditional_t<std::is_convertible<L,D>::value,D,undefined>;
        };
        
        template<class L,class R>
        struct  Impl<L,R,_lvalue_reference,_rvalue_reference>
          :Impl<R,L,_rvalue_reference,_lvalue_reference>
        {};
      
      public:
        using type=typename Impl<A,B>::type;
      };
      
      template<class...Types>
      struct common_reference
      {
      private:
        ///sizeof...(T)==0
        template<class...>
        struct Impl
        {};
        
        template<class Type>
        struct Impl<Type>
          :type_identity<Type>
        {};
        
        template<class A,class B>
        struct Impl<A,B>
        {
        private:
          template<class L,class R,int cond=1,class=void>
          struct Two
            :Two<L,R,cond+1>
          {};
          
          template<class L,class R>
          struct Two<L,R,1,std::enable_if_t<
            std::conjunction_v<std::is_reference<L>,
            std::is_reference<R>>>>
          {
            private:
            using S=typename SimpleCommonReference<L,R>::type;
            public:
            using type=std::conditional_t<
              std::conjunction_v<std::is_convertible<map_t<L,std::add_pointer>,
              map_t<S,std::add_pointer>>,
            std::is_convertible<map_t<L,std::add_pointer>,map_t<S,std::add_pointer>>>,
            S,typename Two<L,R,2>::type>;
            
          };
          
          template<class L,class R>
          struct Two<L,R,2,std::void_t<typename basic_common_reference<remove_cvref_t<L>,
            remove_cvref_t<R>,
            xref<L>::template type,
            xref<R>::template type>::type>>
          :basic_common_reference<remove_cvref_t<L>,
          remove_cvref_t<R>,
          xref<L>::template type,
          xref<R>::template type>
          {};
          
          template<class L,class R>
          struct Two<L,R,3,std::void_t<cond_res_t<L,R>>>
            :cond_res<L,R>
          {};
          
          template<class L,class R>
          struct Two<L,R,4,std::void_t<std::common_type_t<L,R>>>
            :std::common_type<L,R>
          {};
          
          template<class L,class R>
          struct Two<L,R,5>
          {};
        public:
          using type=typename Two<A,B>::type;
        };
        
        template<class A,class B,class C,class...Rest>
        struct Impl<A,B,C,Rest...>
          :Impl<typename Impl<A,B>::type,C,Rest...>
        {};
      
      public:
        using type=typename Impl<Types...>::type;
      };
    }
    
    //std::common_reference
    template<class...Types>
    struct common_reference
      :_common_reference_detail::common_reference<Types...>
    {};
    
  //std::common_reference_t
    template<class...Types>
    using common_reference_t=typename common_reference<Types...>::type;
}
#endif
