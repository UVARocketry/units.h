#ifndef UNITS_UTILS_H
#define UNITS_UTILS_H
#include <type_traits>

template <typename T>
/// Extracts a type out of a class template argument list (eg C<T> -> T)
struct ExtractInnerType {};
template <template <typename R> typename C, typename T>
struct ExtractInnerType<C<T>> {
    typedef T Out;
};

/// Returns true if T is derived from U, eg if T can be expressed as U<Args...>
template <typename T, template <typename...> typename U>
struct DerivedFrom : std::false_type {};
template <typename... T, template <typename...> typename U>
struct DerivedFrom<U<T...>, U> : std::true_type {};

/// Joins Class<ArgsL...> and Class<ArgsR...> into Class<ArgsL..., ArgsR...>
template <typename L, typename R, template <typename...> typename Class,
          class = void>
struct JoinGroups {
    typedef Class<L, R> Out;
};

/// Special case if R is a raw type
template <typename... Args, typename R, template <typename...> typename Class>
struct JoinGroups<Class<Args...>, R, Class,
                  std::enable_if_t<!DerivedFrom<R, Class>::value>> {
    typedef Class<Args..., R> Out;
};

/// Special case if L is a raw type
template <typename... Args, typename L, template <typename...> typename Class>
struct JoinGroups<L, Class<Args...>, Class,
                  std::enable_if_t<!DerivedFrom<L, Class>::value>> {
    typedef Class<L, Args...> Out;
};

template <typename... ArgsL, typename... ArgsR,
          template <typename...> typename Class>
struct JoinGroups<Class<ArgsL...>, Class<ArgsR...>, Class> {
    typedef Class<ArgsL..., ArgsR...> Out;
};
#endif
