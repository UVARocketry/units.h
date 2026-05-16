#ifndef UNITS_SIMPLIFICATIONPASSES_H
#define UNITS_SIMPLIFICATIONPASSES_H
#include "_units_sortTemplatePack.h"
#include "units_defs.h"
#include "units_utils.h"
#include <type_traits>

/// Reduces the operation ordering of T into just numerator and denominator.
/// inNumerator is set to true when the current evaluation context is a
/// numerator (eg the top side of a _Div<> (usually)).
///
/// The outputs of this function are Numerator and Denominator.
/// Numerator and Denominator will ALWAYS be template instances of _Mult
template <typename T, bool inNumerator, class = void>
struct OpRedux {
    /// Base case (eg T=Meter): Numerator is T, Denominator is 1
    /// This requires that inNumerator is true
    // static_assert(inNumerator);
    typedef _Mult<T> Numerator;
    typedef _Mult<One> Denominator;
};

/// Base case (eg T=Meter): Numerator is 1, Denominator is T
/// when isNumerator is false
template <typename T>
struct OpRedux<T, false,
               std::enable_if_t<!DerivedFrom<T, _Mult>::value &&
                                !DerivedFrom<T, _Div>::value>> {
    typedef _Mult<One> Numerator;
    typedef _Mult<T> Denominator;
};

/// For divisions
template <typename T1, typename T2, bool inNumerator>
struct OpRedux<_Div<T1, T2>, inNumerator> {
    /// We need to reduce the top and bottom, the top is the same numerator-ness
    /// as the current evaluation context
    typedef OpRedux<T1, inNumerator> RTop;
    /// The bottom is the opposite numerator-ness as the current evaluation
    /// context
    typedef OpRedux<T2, !inNumerator> RBottom;

    /// Join the numerators specified by the top and bottom reductions
    typedef typename RTop::Numerator NR;
    typedef typename RBottom::Numerator NB;
    typedef typename JoinGroups<NR, NB, _Mult>::Out Numerator;

    /// Join the denominators specified by the top and bottom reductions
    typedef typename RTop::Denominator DR;
    typedef typename RBottom::Denominator DB;
    typedef typename JoinGroups<DR, DB, _Mult>::Out Denominator;
};

/// For multiplications, we need to reduce each element
template <typename T, typename... Args, bool inNumerator>
struct OpRedux<_Mult<T, Args...>, inNumerator> {
    /// Reduce the first element.
    /// Mult doesnt change the evaluation context like div, so pass isNumerator
    /// to both L and R
    typedef OpRedux<T, inNumerator> L;
    /// Reduce the rest
    typedef OpRedux<_Mult<Args...>, inNumerator> R;
    // join the numerators
    typedef typename L::Numerator LN;
    typedef typename R::Numerator RN;
    typedef typename JoinGroups<LN, RN, _Mult>::Out Numerator;

    // join the denominators
    typedef typename L::Denominator LD;
    typedef typename R::Denominator RD;
    typedef typename JoinGroups<LD, RD, _Mult>::Out Denominator;
};

/// reduces nested divs and stuff into div(mult(), mult())
template <typename T>
struct ReduceOperations {
    /// Reduce the operations
    typedef OpRedux<T, true> Reduction;

    /// Sort the numerator
    typedef typename Reduction::Numerator Numerator;
    // OpRedux only returns _Mult<> for numerator and denominator so it's safe
    // to just sort the numerator and denominator parameter packs without
    // checking whether they are a valid instance of _Mult<...>
    typedef typename Sort<Numerator>::Out NumeratorSorted;

    /// Sort the denominator
    typedef typename Reduction::Denominator Denominator;
    typedef typename Sort<Denominator>::Out DenominatorSorted;

    /// Set up the actual div(mult(), mult())
    typedef _Div<NumeratorSorted, DenominatorSorted> Out;
};

/// A class that removes the matching elements in two _Mult<...> parameter packs
///
/// Outputs two types: T and B (for Top and Bottom, respectively). These will
/// both be instances of _Mult<...>
template <typename Top, typename Bottom, class = void>
struct RemoveMatchingSub {
    typedef Top T;
    typedef Bottom B;
};

template <typename Top, typename Bottom, typename... RestTop,
          typename... RestBottom>
struct RemoveMatchingSub<
    _Mult<Top, RestTop...>, _Mult<Bottom, RestBottom...>,
    std::enable_if_t<(typeHash<Bottom>() < typeHash<Top>() &&
                      !std::is_same_v<Top, Bottom>)>> {
    // if hash(Bottom) < hash(Top) it means that, to find a matching bottom,
    // we need to iterate forward on Bottom because the arrays are sorted
    // smallest to largest
    //
    // eg if T=[2, 3, 4] and B=[1, 2, 3] we need to iterate forward on B if we
    // want to find a match
    typedef RemoveMatchingSub<_Mult<Top, RestTop...>, _Mult<RestBottom...>>
        RemovedRest;

    /// Since we didnt do any fiddling with the Top section, we just forward it
    typedef typename RemovedRest::T T;
    /// Prepend Bottom to the simplified rest of the bottom
    typedef typename JoinGroups<Bottom, typename RemovedRest::B, _Mult>::Out B;
};

template <typename Top, typename Bottom, typename... RestTop,
          typename... RestBottom>
struct RemoveMatchingSub<
    _Mult<Top, RestTop...>, _Mult<Bottom, RestBottom...>,
    std::enable_if_t<(typeHash<Bottom>() >= typeHash<Top>() &&
                      !std::is_same_v<Top, Bottom>)>> {
    // if hash(Bottom) >= hash(Top) it means that to find a matching bottom,
    // we need to iterate forward on Top
    typedef RemoveMatchingSub<_Mult<RestTop...>, _Mult<Bottom, RestBottom...>>
        RemovedRest;
    typedef typename JoinGroups<Top, typename RemovedRest::T, _Mult>::Out T;
    typedef typename RemovedRest::B B;
};

/// If we have a matching element
template <typename V, typename... RestTop, typename... RestBottom>
struct RemoveMatchingSub<_Mult<V, RestTop...>, _Mult<V, RestBottom...>> {
    /// skip the matching element
    typedef RemoveMatchingSub<_Mult<RestTop...>, _Mult<RestBottom...>>
        RemovedRest;
    /// forward the rest
    typedef typename RemovedRest::T T;
    typedef typename RemovedRest::B B;
};

template <typename T, class = void>
struct RemoveMatching {
    typedef T Out;
};

template <typename Top, typename Bottom>
struct RemoveMatching<_Div<Top, Bottom>> {
    /// filter out pairs in top and bottom
    typedef RemoveMatchingSub<Top, Bottom> Simple;
    typedef _Div<typename Simple::T, typename Simple::B> Out;
};

template <typename T, class = void>
struct RemoveGarbageSub {
    typedef T Out;
};

/// a _Mult<One, ...> is just a _Mult<...>
template <typename... Args>
struct RemoveGarbageSub<_Mult<One, Args...>> {
    typedef typename RemoveGarbageSub<_Mult<Args...>>::Out Out;
};

/// a _Mult<> is just a One in disguise
template <typename... Args>
struct RemoveGarbageSub<_Mult<_Mult<>, Args...>> {
    typedef typename RemoveGarbageSub<_Mult<Args...>>::Out Out;
};

/// when the element isnt _Mult<> or One, remove the garbage from the rest of
/// the pack and then add this to the output
template <typename V, typename... Args>
struct RemoveGarbageSub<
    _Mult<V, Args...>,
    std::enable_if_t<!std::is_same_v<V, _Mult<>> && !std::is_same_v<V, One>>> {
    typedef typename RemoveGarbageSub<_Mult<Args...>>::Out Removed;
    typedef typename JoinGroups<V, Removed, _Mult>::Out Out;
};

/// first remove garbage step
template <typename T, class = void>
struct RemoveGarbage {
    typedef T Out;
};

template <typename L, typename R>
struct RemoveGarbage<_Div<L, R>> {
    typedef typename RemoveGarbageSub<L>::Out Top;
    typedef typename RemoveGarbageSub<R>::Out Bottom;
    typedef _Div<Top, Bottom> Out;
};

/// The second remove garbage step converts _Mult<..., _Mult<T>, ...> into
/// _Mult<..., T, ...>

template <typename T>
struct ExtractMultMaybe {
    typedef T Out;
};
template <typename T>
struct ExtractMultMaybe<_Mult<T>> {
    typedef T Out;
};

template <typename T, class = void>
struct RemoveGarbage2;

template <typename... ArgsL, typename... ArgsR>
struct RemoveGarbage2<_Div<_Mult<ArgsL...>, _Mult<ArgsR...>>> {
    typedef _Mult<typename ExtractMultMaybe<ArgsL>::Out...> Top;
    typedef _Mult<typename ExtractMultMaybe<ArgsR>::Out...> Bottom;
    typedef _Div<Top, Bottom> Out;
};

/// Reduce simplifies any remaining unnecessary operations (eg T/1)
template <typename T, class = void>
struct Reduce {
    typedef T Value;
};

/// x/1 = x
template <typename T>
struct Reduce<_Div<T, One>> {
    /// make sure we continue reducing just in case there's nested simple
    /// operations (eg _Div<_Mult<One>, One>)
    typedef typename Reduce<T>::Value Value;
};

/// _Mult<> = One
template <>
struct Reduce<_Mult<>> {
    typedef One Value;
};

/// The following three reductions are for unpacking:
///
/// _Div<_Mult<T>, _Mult<V>> into _Div<T, V>
///
/// AKA: _Div<_Mult<Meter>, _Mult<Second>> -> _Div<Meter, Second>
///
/// I have to specialize for all 3 combinations of raw types and _Mult<T>
/// bc otherwise we get an ambiguous specialization error
/// (eg: _Div<T, _Mult<V>>)
template <typename U, typename T>
struct Reduce<_Div<_Mult<T>, _Mult<U>>> {
    typedef typename Reduce<_Div<T, U>>::Value Value;
};
template <typename U, typename T>
struct Reduce<_Div<_Mult<T>, U>,
              std::enable_if_t<!DerivedFrom<U, _Mult>::value>> {
    typedef typename Reduce<_Div<T, U>>::Value Value;
};
template <typename U, typename T>
struct Reduce<_Div<U, _Mult<T>>,
              std::enable_if_t<!DerivedFrom<U, _Mult>::value>> {
    typedef typename Reduce<_Div<U, T>>::Value Value;
};

/// _Mult<T> is just T
template <typename T>
struct Reduce<_Mult<T>> {
    typedef typename Reduce<T>::Value Value;
};

/// this is x/1 in disguise
template <typename T>
struct Reduce<_Div<T, _Mult<>>, std::enable_if_t<!std::is_same_v<T, _Mult<>>>> {
    typedef typename Reduce<T>::Value Value;
};

/// a leftover artifact from reduction can be: _Div<_Mult<>, T>.
/// Reduce that to _Div<One, T> so that errors are more understandable
template <typename T>
struct Reduce<_Div<_Mult<>, T>> {
    typedef typename Reduce<_Div<One, T>>::Value Value;
};

/// Converts Quantity<T> to T
template <typename T>
struct ExtractQuantity {
    typedef T Value;
};

template <typename T>
struct Quantity;

template <typename T>
struct ExtractQuantity<Quantity<T>> {
    typedef T Value;
};

template <typename T>
using ExtractQuantityT = typename ExtractQuantity<T>::Value;

/// Takes a _Mult<...> or _Div<> and extracts any Quantity<T> into T for every
/// value
template <typename T>
struct Extract;

template <typename... Args>
struct Extract<_Mult<Args...>> {
    typedef _Mult<typename ExtractQuantity<Args>::Value...> Out;
};

template <typename... Args>
struct Extract<_Div<Args...>> {
    typedef _Div<typename ExtractQuantity<Args>::Value...> Out;
};

/// The actual simplification call
template <typename T>
struct SimplifyQuantity {
    typedef typename Extract<T>::Out Mult;
    typedef typename ReduceOperations<Mult>::Out Reduction;
    typedef typename RemoveMatching<Reduction>::Out Simple;
    typedef typename RemoveGarbage<Simple>::Out Clean;
    typedef typename RemoveGarbage2<Clean>::Out Clean2;
    typedef typename Reduce<Clean2>::Value Out;
};

template <typename... A>
using Mult = typename SimplifyQuantity<_Mult<A...>>::Out;
template <typename A, typename B>
using Div = typename SimplifyQuantity<_Div<A, B>>::Out;

template <typename T>
struct MultWrap {
    typedef _Mult<T> Out;
};
template <>
struct MultWrap<One> {
    typedef _Mult<> Out;
};
template <typename... Args>
struct MultWrap<_Mult<Args...>> {
    typedef _Mult<Args...> Out;
};

template <typename T, class = void>
struct CommonForm {
    typedef typename MultWrap<T>::Out Numerator;
    typedef _Mult<> Denominator;
};

template <typename T, typename B>
struct CommonForm<_Div<T, B>> {
    typedef typename MultWrap<T>::Out Numerator;
    typedef typename MultWrap<B>::Out Denominator;
};

template <typename... Args>
struct CommonForm<_Mult<Args...>> {
    typedef _Mult<Args...> Numerator;
    typedef _Mult<> Denominator;
};

/// The simplification for operations on ALREADY SIMPLIFIED quantities
/// Intended to be used only by Quantity internally
template <typename T1, typename T2>
struct SimpleDiv {
    typedef CommonForm<T1> Top;
    typedef CommonForm<T2> Bottom;

    typedef RemoveMatchingSub<typename Top::Numerator,
                              typename Bottom::Numerator>
        TopRemoval;

    typedef RemoveMatchingSub<typename Top::Denominator,
                              typename Bottom::Denominator>
        BottomRemoval;

    typedef
        typename Merge<typename TopRemoval::T, typename BottomRemoval::B>::Out
            Numerator;
    typedef
        typename Merge<typename TopRemoval::B, typename BottomRemoval::T>::Out
            Denominator;

    typedef _Div<Numerator, Denominator> D;
    typedef typename Reduce<D>::Value Out;
};

/// The simplification for operations on ALREADY SIMPLIFIED quantities
/// Intended to be used only by Quantity internally
template <typename T1, typename T2>
struct SimpleMult {
    typedef CommonForm<T1> Left;
    typedef CommonForm<T2> Right;

    typedef RemoveMatchingSub<typename Left::Numerator,
                              typename Right::Denominator>
        LeftRemoval;

    typedef RemoveMatchingSub<typename Right::Numerator,
                              typename Left::Denominator>
        RightRemoval;

    typedef
        typename Merge<typename LeftRemoval::T, typename RightRemoval::T>::Out
            Numerator;
    typedef
        typename Merge<typename LeftRemoval::B, typename RightRemoval::B>::Out
            Denominator;

    typedef _Div<Numerator, Denominator> D;
    typedef typename Reduce<D>::Value Out;
};

template <typename A, typename B>
using Mult2 = typename SimpleMult<A, B>::Out;
template <typename A, typename B>
using Div2 = typename SimpleDiv<A, B>::Out;
#endif
