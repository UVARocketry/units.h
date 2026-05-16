#ifndef UNITS_CONVERSIONS_H
#define UNITS_CONVERSIONS_H
#include "units_defs.h"
#include <cstddef>
#include <tuple>
#include <type_traits>
struct ConversionFactor {
    // most things should assume add is 0 tbh
    float add;
    float mult;

    constexpr ConversionFactor(float add, float mult) : add(add), mult(mult) {
    }

    constexpr ConversionFactor(float mult) : add(0.0), mult(mult) {
    }

    constexpr inline float applyTo(float v) const {
        return (v + add) * mult;
    }
    constexpr inline float applyInverseTo(float v) const {
        return v / mult - add;
    }
};

template <typename T, class = void>
struct HasForwardingConversion : std::false_type {};

template <typename T>
struct HasForwardingConversion<T, std::void_t<typename T::ConversionTo>>
  : std::true_type {};

template <typename T>
constexpr bool has_forwarding_conversion_v = HasForwardingConversion<T>::value;

/// ForwardingConversion for scalar types (non-_Mult, non-_Div)
template <typename T, class = void>
struct ForwardingConversionImpl {
    typedef T Forward;
};

template <typename T>
struct ForwardingConversionImpl<
    T, std::enable_if_t<has_forwarding_conversion_v<T>>> {
    typedef typename T::ConversionTo Forward;
};

/// ForwardingConversion for _Mult - tries first arg, then rest
template <typename... Args>
struct ForwardingConversionMult;

template <typename First, typename... Rest>
struct ForwardingConversionMult<First, Rest...> {
    typedef typename ForwardingConversionImpl<First>::Forward FirstForward;
    typedef typename ForwardingConversionMult<Rest...>::Forward RestForward;
    // Use First's forwarding if it's different from First
    typedef std::conditional_t<!std::is_same_v<First, FirstForward>,
                               _Mult<FirstForward, Rest...>,
                               _Mult<First, RestForward>>
        Forward;
};

template <typename Last>
struct ForwardingConversionMult<Last> {
    typedef typename ForwardingConversionImpl<Last>::Forward Forward;
};

template <>
struct ForwardingConversionMult<> {
    typedef _Mult<> Forward;
};

/// ForwardingConversion for _Div - tries numerator first, then denominator
template <typename Num, typename Den>
struct ForwardingConversionDiv {
    typedef typename ForwardingConversionImpl<Num>::Forward NumForward;
    typedef typename ForwardingConversionImpl<Den>::Forward DenForward;
    // Use numerator's forwarding if it forwards somewhere
    typedef std::conditional_t<!std::is_same_v<Num, NumForward>,
                               _Div<NumForward, Den>, _Div<Num, DenForward>>
        Forward;
};

/// Main ForwardingConversion dispatch
template <typename T, class = void>
struct ForwardingConversion : ForwardingConversionImpl<T> {};

template <typename... Args>
struct ForwardingConversion<_Mult<Args...>, void>
  : ForwardingConversionMult<Args...> {};

template <typename Num, typename Den>
struct ForwardingConversion<_Div<Num, Den>, void>
  : ForwardingConversionDiv<Num, Den> {};

template <typename T, class = void>
struct CommonType {
    typedef T Out;
};
template <typename T>
struct CommonType<T, std::enable_if_t<has_forwarding_conversion_v<T>>> {
    typedef typename CommonType<typename T::ConversionTo>::Out Out;
};

template <typename T>
using common_type_t = typename CommonType<T>::Out;

template <typename T1, typename T2, class = void>
struct ConversionTo {
    typedef typename ForwardingConversion<T1>::Forward T1Forward;
    typedef typename ForwardingConversion<T2>::Forward T2Forward;

    constexpr static const inline bool noForwards =
        std::is_same_v<T1Forward, T1> && std::is_same_v<T2Forward, T2>;
    static_assert(!noForwards, "No viable conversion found!");

    typedef ConversionTo<T1, T1Forward> T1Conversion;
    typedef ConversionTo<T2Forward, T2> T2Conversion;
    typedef ConversionTo<T1Forward, T2Forward> Sub;

    static inline constexpr float apply(float v) {
        float to = T1Conversion::apply(v);
        float mid = Sub::apply(to);
        return T2Conversion::apply(mid);
    }
};

template <>
struct ConversionTo<_Mult<>, _Mult<>> {
    static inline constexpr float apply(float v) {
        return v;
    }
};

template <typename T>
struct ConversionTo<T, typename T::ConversionTo> {
    static inline constexpr float apply(float v) {
        return T::factor.applyTo(v);
    }
};

template <typename T>
struct ConversionTo<T, T> {
    static inline constexpr float apply(float v) {
        return v;
    }
};

template <typename T>
struct ConversionTo<typename T::ConversionTo, T> {
    static inline constexpr float apply(float v) {
        return T::factor.applyInverseTo(v);
    }
};

/// Helper to find the index of a type in a pack that has the same common type
/// as Target
template <typename Target, typename... Pack>
struct FindMatchingCommonType;

template <typename Target, typename First, typename... Rest>
struct FindMatchingCommonType<Target, First, Rest...> {
    static constexpr size_t value =
        std::is_same_v<common_type_t<Target>, common_type_t<First>>
            ? 0
            : 1 + FindMatchingCommonType<Target, Rest...>::value;
};

template <typename Target>
struct FindMatchingCommonType<Target> {
    static constexpr size_t value = 0; // Not found, will cause out-of-bounds
};

/// Helper to get Nth type from a pack
template <size_t N, typename... Pack>
struct PackElement {
    using type = std::tuple_element_t<N, std::tuple<Pack...>>;
};

/// ConversionTo for _Mult - matches by common type, not position
/// _Mult<A, B> to _Mult<C, D> finds pairs with matching common types
template <typename... FromArgs, typename... ToArgs>
struct ConversionTo<
    _Mult<FromArgs...>, _Mult<ToArgs...>,
    std::enable_if_t<sizeof...(FromArgs) == sizeof...(ToArgs) &&
                     !std::is_same_v<_Mult<FromArgs...>, _Mult<ToArgs...>>>> {

    // Get conversion factor for a single From type to its matching To type
    template <typename FromType>
    static float getFactorForFromType() {
        constexpr size_t toIndex =
            FindMatchingCommonType<FromType, ToArgs...>::value;
        using ToType = typename PackElement<toIndex, ToArgs...>::type;
        static_assert(toIndex < sizeof...(ToArgs),
                      "No matching common type found");
        return ConversionTo<FromType, ToType>::apply(1.0f);
    }

    // Recursively multiply all factors
    template <typename... FromTypes>
    static float applyForAll(float v) {
        return v * (getFactorForFromType<FromTypes>() * ... * 1.0f);
    }

    static float apply(float v) {
        return applyForAll<FromArgs...>(v);
    }
};

/// ConversionTo for _Div - numerator and denominator conversion
/// Delegates to the underlying type conversions which handle sorting
template <typename FromNum, typename FromDen, typename ToNum, typename ToDen>
struct ConversionTo<_Div<FromNum, FromDen>, _Div<ToNum, ToDen>,
                    std::enable_if_t<!std::is_same_v<_Div<FromNum, FromDen>,
                                                     _Div<ToNum, ToDen>>>> {
    static float apply(float v) {
        float numFactor = ConversionTo<FromNum, ToNum>::apply(1.0f);
        float denFactor = ConversionTo<FromDen, ToDen>::apply(1.0f);
        return v * numFactor / denFactor;
    }
};
#endif
