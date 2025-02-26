#ifndef INVOKE_HPP
#define INVOKE_HPP

#include "../meta/meta.hpp"

#define INVOKE_NOEXCEPT_DECLTYPE_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__)) -> decltype (__VA_ARGS__) { return __VA_ARGS__; }

namespace meta
{
    template<template<class...>class T,class>
    struct _is_specialization
        :std::false_type
    {};
    
    template<template<class...>class T,class...Types>
    struct _is_specialization<T,T<Types...>>
        :std::true_type
    {};
    
    template<template<class...>class TL,class T>
    struct is_specialization
        :_is_specialization<TL,remove_cvref_t<T>>
    {};
}

namespace meta
{
    // member_object_pointer
    template<class Class,class Callable,class Obj,
        class=std::enable_if_t<std::is_base_of<Class,remove_cvref_t<Obj>>::value
                               ||std::is_same<Class,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mop(Callable Class::*callable, Obj&& obj)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(static_cast<Obj&&>(obj).*callable)
    
    template<class Class,class Callable,class Obj,class...Args,
        class=std::enable_if_t<is_specialization<std::reference_wrapper,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mop(Callable Class::*callable,Obj&&obj)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(obj.get().*callable)
    
    template<class Class,class Callable,class Obj,
        class=std::enable_if_t<
            !std::is_base_of<Class,remove_cvref_t<Obj>>::value &&
            !is_specialization<std::reference_wrapper,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mop(Callable Class::* callable,Obj&&obj)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN((*static_cast<Obj&&>(obj)).*callable)
    
    // member_function_pointer
    template<class Class,class Callable,class Obj,class...Args,
        class=std::enable_if_t<std::is_base_of<Class,remove_cvref_t<Obj>>::value
                               ||std::is_same<Class,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mfp(Callable Class::*callable,Obj&& obj,Args&&...args)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN((static_cast<Obj&&>(obj).*callable)(static_cast<Args&&>(args)...))
    
    template<class Class,class Callable,class Obj,class...Args,
        class=std::enable_if_t<is_specialization<std::reference_wrapper,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mfp(Callable Class::*callable,Obj&&obj,Args&&...args)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN((obj.get().*callable)(static_cast<Args&&>(args)...))
    
    template<class Class,class Callable,class Obj,class...Args,
        class=std::enable_if_t<
            !std::is_base_of<Class,remove_cvref_t<Obj>>::value &&
            !is_specialization<std::reference_wrapper,remove_cvref_t<Obj>>::value>>
    constexpr auto _invoke_mfp(Callable Class::*callable,Obj&&obj,Args&&...args)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(((*static_cast<Obj&&>(obj)).*callable)(static_cast<Args&&>(args)...))
    
    template<class Callable,class... Args,
        class= std::enable_if_t<!std::is_member_pointer<remove_cvref_t<Callable>>::value>>
    constexpr auto invoke(Callable&&callable,Args&&... args)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(static_cast<Callable&&>(callable)(static_cast<Args&&>(args)...))
    
    template<class Callable,class Obj,
        class=std::enable_if_t<std::is_member_object_pointer<remove_cvref_t<Callable>>::value>>
    constexpr auto invoke(Callable&& callable,Obj&&obj)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(_invoke_mop(static_cast<Callable&&>(callable),static_cast<Obj&&>(obj)))
    
    template<class Callable,class Obj,class...Args,
        class=std::enable_if_t<std::is_member_function_pointer<remove_cvref_t<Callable>>::value>>
    constexpr auto invoke(Callable&& callable,Obj&&obj,Args&&...args)
    INVOKE_NOEXCEPT_DECLTYPE_RETURN(_invoke_mfp(static_cast<Callable&&>(callable),static_cast<Obj&&>(obj),static_cast<Args&&>(args)...))
}

namespace meta
{
    template < class Void, class Callable, class... Args >
    struct _invoke_result {};
    
    template < class Callable, class... Args >
    struct _invoke_result<std::void_t<decltype(meta::invoke(std::declval<Callable>(), std::declval<Args>()...))>,
        Callable, Args...>
    {
        using type = decltype(meta::invoke(std::declval<Callable>(), std::declval<Args>()...));
    };
    
    
    template < class Callable,class... Args >
    struct invoke_result
        : _invoke_result<void,Callable,Args...> {};
    
    template < class Callable,class... Args >
    using invoke_result_t = typename invoke_result<Callable, Args...>::type;
}

namespace meta
{
    template < class AlwaysVoid ,class ,class,class... >
    struct _is_invocable_r
        : std::false_type
    {};
    
    template < class Return, class Callable, class... Args >
    struct _is_invocable_r<std::void_t<invoke_result_t<Callable, Args...>>, Return, Callable, Args...>
        : std::conditional_t<std::is_void<Return>::value,
            std::true_type,
            std::is_convertible<invoke_result_t<Callable, Args...>, Return>>
    {};
    
    template < class Return,class Callable, class... Args >
    struct is_invocable_r
        : _is_invocable_r<void, Return, Callable, Args...>
    {};
    
    template<class Void,class Return,class Callable,class...Args>
    struct _is_nothrow_invocable_r
        :std::false_type
    {};
    
    template<class Return,class Callable,class...Args>
    struct _is_nothrow_invocable_r<std::enable_if_t<noexcept(invoke(std::declval<Callable>(),std::declval<Args>()...))>,Return,Callable,Args...>
        :std::integral_constant<bool,noexcept(invoke(std::declval<Callable>(),std::declval<Args>()...))>
    {};
    
    template<class Return,class Callable,class...Args>
    struct is_nothrow_invocable_r
        :std::integral_constant<bool,_is_nothrow_invocable_r<Return,Callable,Args...>::value
                                     &&is_invocable_r<Return,Callable,Args...>::value>
    {};
    
    template < class Callable, class... Args >
    using is_nothrow_invocable = is_nothrow_invocable_r<void, Callable, Args...>;
    
    template < class Callable, class... Args >
    using is_invocable = is_invocable_r<void, Callable, Args...>;
    
    template<class R, class F, class... Args,
        class=std::enable_if_t<is_invocable_r<R, F, Args...>::value>>
    constexpr R invoke_r(F&& f, Args&&... args)
    noexcept(is_nothrow_invocable_r<R, F, Args...>::value)
    {
        return static_cast<R>(meta::invoke(std::forward<F>(f), std::forward<Args>(args)...));
    }
    
    template<class R,class Callable,class...Args>
    inline constexpr bool is_invocable_r_v=is_invocable_r<R,Callable,Args...>::value;
    
    template<class Callable,class...Args>
    inline constexpr bool is_invocable_v=is_invocable<Callable,Args...>::value;
    
    template<class R,class Callable,class...Args>
    inline constexpr bool is_nothrow_invocable_r_v=is_nothrow_invocable_r<R,Callable,Args...>::value;
    
    template<class Callable,class...Args>
    inline constexpr bool is_nothrow_invocable_v=is_nothrow_invocable<Callable,Args...>::value;
    
    /*
    auto func2(char) -> int (*)()
    {
      return nullptr;
    }
    
    static_assert(is_invocable_v<int()>);
    static_assert(not is_invocable_v<int(), int>);
    static_assert(is_invocable_r_v<int, int()>);
    static_assert(not is_invocable_r_v<int*, int()>);
    static_assert(is_invocable_r_v<void, void(int), int>);
    static_assert(not is_invocable_r_v<void, void(int), void>);
    static_assert(is_invocable_r_v<int(*)(), decltype(func2), char>);
    static_assert(not is_invocable_r_v<int(*)(), decltype(func2), void>);
    */
    
}

#undef INVOKE_NOEXCEPT_DECLTYPE_RETURN
#endif
