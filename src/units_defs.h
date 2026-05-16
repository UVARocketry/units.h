#ifndef UNITS_DEFS_H
#define UNITS_DEFS_H
// TODO: add a Sqrt<>
// also be able to simplify Sqrt<>s (something in the reduction step that finds
// n groups of things and calls Sqrt<T>::SimplifyWith<n> or something)

// TODO: also take Sqrt<N, N, ...> and turn it into _Mult<N, Sqrt<...>>

// TODO: also generalize roots?

template <typename... Args>
struct _Mult {};

template <typename L, typename R>
struct _Div {};

struct One {
    static constexpr inline const char* unitName = "";
};

#endif
