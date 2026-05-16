#ifndef UNITS_SORTTEMPLATEPACK_H
#define UNITS_SORTTEMPLATEPACK_H
/*
 * This file contains utilities for sorting template classes with parameter
 * packs of type parameters.
 *
 * The primary export is Sort<T<...>> which sorts the template class T's
 * parameters
 *
 * Eg: Sort<Class<T1, T2, T5, T4, T3>>::Out == Class<T1, T2, T3, T4, T5>
 *
 * Here be dragons
 */

#include "units_stringView.h"
#include <cstdint>
#include <type_traits>

template <typename T>
/// How to get the name of a type at constexpr time. Yes I know this is
/// horrifying
constexpr StringView typeName() noexcept {
    StringView name = StringView::fromRawString("Error: unsupported compiler"),
               prefix, suffix;
#ifdef __clang__
    name = StringView::fromRawString(__PRETTY_FUNCTION__);
    prefix = StringView::fromRawString("StringView type_name() [T = ");
    suffix = StringView::fromRawString("]");
#elif defined(__GNUC__)
    name = StringView::fromRawString(__PRETTY_FUNCTION__);
    prefix =
        StringView::fromRawString("constexpr uint64_t type_name() [with T = ");
    suffix = StringView::fromRawString("]");
// #elif defined(_MSC_VER)
//     name = __FUNCSIG__;
//     prefix = "auto __cdecl type_name<";
//     suffix = ">(void) noexcept";
#else
    static_assert(false, "Unsupported compiler!");
#endif

    name = name.removePrefix(prefix.size() - 1);
    name = name.removeSuffix(suffix.size());
    return name;
}

template <typename T>
/// (hopefully) returns a unique number for each type.
/// TODO: can we get a better hashing algo?
constexpr uint64_t typeHash() noexcept {
    StringView name = typeName<T>();
    uint64_t ret = 0;
    // essentially we split the string into 8byte blocks and XOR them together
    for (size_t i = 0; i < name.size(); i += 8) {
        uint64_t block = 0;
        for (size_t j = i; j < name.size() && j < i + 8; j++) {
            block <<= 8;
            block += name[i];
        }
        ret ^= block;
    }
    return ret;
}

/// Pushes the first parameter as the first template parameter for the
/// pack of the second parameter.
///
/// Essentially, it takes T and Class<Values...> and returns Class<T, Values...>
template <typename L, typename R>
struct Prepend;

template <typename L, template <typename...> typename Class, typename... Rest>
struct Prepend<L, Class<Rest...>> {
    typedef Class<L, Rest...> Out;
};

/// Returns the number of parameters in a parameter pack.
/// Eg: Pack<int, int> -> 2, Pack<> -> 0, Pack<int, int, int> -> 3
template <typename... Args>
struct PackLength {
    static constexpr int len = 0;
};

template <typename T, typename... Args>
struct PackLength<T, Args...> {
    static constexpr int len = PackLength<Args...>::len + 1;
};

/// The "merge" step of mergesort. Takes two Class<...> types and joins them
/// into one Class<...> with the types with the lowest hash first
template <typename L, typename R, class = void>
struct Merge;

/// takes [L, LRest...] and [R, RRest...] IFF L is the smaller hashed item
/// and returns [L, merge([LRest...], [R, RRest...])...]
template <template <typename...> typename Class, typename L, typename R,
          typename... RestLeft, typename... RestRight>
struct Merge<Class<L, RestLeft...>, Class<R, RestRight...>,
             std::enable_if_t<(typeHash<L>() < typeHash<R>())>> {
    typedef typename Prepend<
        L, typename Merge<Class<RestLeft...>, Class<R, RestRight...>>::Out>::Out
        Out;
};

/// takes [L, LRest...] and [R, RRest...] IFF L is NOT the smaller hashed item
/// and returns [R, merge([L, LRest...], [RRest...])...]
template <template <typename...> typename Class, typename L, typename R,
          typename... RestLeft, typename... RestRight>
struct Merge<Class<L, RestLeft...>, Class<R, RestRight...>,
             std::enable_if_t<(typeHash<L>() >= typeHash<R>())>> {
    typedef
        typename Prepend<R, typename Merge<Class<L, RestLeft...>,
                                           Class<RestRight...>>::Out>::Out Out;
};

/// Base case for merge: if one side is empty, we dont need to do any more
/// sorting
template <template <typename...> typename Class, typename... RestLeft>
struct Merge<Class<RestLeft...>, Class<>> {
    typedef Class<RestLeft...> Out;
};

/// Base case for merge: if one side is empty, we dont need to do any more
/// sorting
template <template <typename...> typename Class, typename R,
          typename... RestRight>
struct Merge<Class<>, Class<R, RestRight...>> {
    typedef Class<R, RestRight...> Out;
};

/// Takes Class<args...> and splits it into Class<left...> and Class<right...>
/// where left... and right... are the same size. Callers should pass [index]
/// as 0, and [len] as PackLength<args...>::len
template <typename V, int index, int len, class = void>
struct Split;

/// If we are still iterating through left side, we take one item
/// and then keep splitting the rest
template <template <typename...> typename Class, typename T, typename... Rest,
          int index, int len>
struct Split<Class<T, Rest...>, index, len,
             std::enable_if_t<(index < len / 2)>> {
    /// Rest split up
    typedef Split<Class<Rest...>, index + 1, len> SplitRest;

    /// Join current value with rest of left
    typedef typename Prepend<T, typename SplitRest::Left>::Out Left;
    /// Return the right side
    typedef typename SplitRest::Right Right;
};

/// If we have iterated through entire left side, we just return the right side
template <template <typename...> typename Class, typename... Rest, int index,
          int len>
struct Split<Class<Rest...>, index, len, std::enable_if_t<(index >= len / 2)>> {
    /// Empty array for left
    typedef Class<> Left;
    /// Return right side
    typedef Class<Rest...> Right;
};

template <typename T>
struct Sort;

/// Sort an empty array does nothing
template <template <typename...> typename Class>
struct Sort<Class<>> {
    typedef Class<> Out;
};

/// Sort a one-element array does nothing
template <template <typename...> typename Class, typename T>
struct Sort<Class<T>> {
    typedef Class<T> Out;
};

/// Actual sort
template <template <typename...> typename Class, typename... Args>
struct Sort<Class<Args...>> {
    /// Split in half
    typedef Split<Class<Args...>, 0, PackLength<Args...>::len> split;
    /// sort both sides
    typedef typename Sort<typename split::Left>::Out LeftSort;
    typedef typename Sort<typename split::Right>::Out RightSort;

    /// merge together
    typedef typename Merge<LeftSort, RightSort>::Out Out;
};
#endif
