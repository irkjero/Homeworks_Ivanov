//
// Created by Петр on 27.12.2020.
//
#include <functional>

#ifndef STREAMAPI_STREAMOPERATORS_H
#define STREAMAPI_STREAMOPERATORS_H

#include <iostream>
#include "Stream.h"
#include "StreamOperations.h"
#include "StreamTypes.h"
#include "StreamProvider.h"

using namespace provider;

template<typename Predicate>
inline auto filter(Predicate&& predicate) {
    return make_operator("stream::op::filter", [=](auto&& stream) mutable {
        using T = StreamType<decltype(stream)>;
        return Stream<T>(std::move(
                make_stream_provider<Filter, T, Predicate>(
                        std::move(stream.getSource()), std::forward<Predicate>(predicate))));
    });
}

inline auto filter() {
    return filter([](bool b){ return b; });
}

inline auto slice(std::size_t start, std::size_t end, std::size_t increment = 1) {
    return make_operator("stream::op::slice", [=](auto&& stream) {
        using T = StreamType<decltype(stream)>;
        return Stream<T>(std::move(
                make_stream_provider<Slice, T>(
                        std::move(stream.getSource()), start, end, increment, false)));
    });
}

inline auto slice_to_end(std::size_t start, std::size_t increment) {
    return make_operator("stream::op::slice", [=](auto&& stream) {
        using T = StreamType<decltype(stream)>;
        return Stream<T>(std::move(
                make_stream_provider<Slice, T>(
                        std::move(stream.getSource()), start, 0, increment, true)));
    });
}

inline auto limit(std::size_t length) {
    return slice(0, length).rename("stream::op::limit");
}

inline auto skip(std::size_t amount) {
    return slice_to_end(amount, 1).rename("stream::op::skip");
}

template<typename Function>
inline auto map_(Function&& function) {
    return make_operator("stream::op::map_", [=](auto&& stream) mutable {
        using T = StreamType<decltype(stream)>;
        using Result = std::result_of_t<Function(T&&)>;
        static_assert(!std::is_void<Result>::value,
                      "Return type of the mapping function cannot be void.");

        return Stream<Result>(std::move(
                make_stream_provider<Map, Result, Function, T>(
                        std::move(stream.getSource()), std::forward<Function>(function))));
    });
}

template<size_t N>
inline auto group() {
    return make_operator("stream::op::group", [=](auto&& stream) {
        using T = StreamType<decltype(stream)>;
        using G = GroupResult<T, N>;
        return Stream<G>(std::move(StreamProviderPtr<G>(
                new Group<T, N>(std::move(stream.getSource())))));
    });
}

inline auto pairwise() {
    return group<2>().rename("stream::op::pairwise");
}

inline auto group(size_t N) {
    return make_operator("stream::op::group", [=](auto&& stream) {
        using T = StreamType<decltype(stream)>;
        using G = std::vector<T>;
        return Stream<G>(std::move(StreamProviderPtr<G>(
                new DynamicGroup<T>(std::move(stream.getSource()), N))));
    });
}

#endif //STREAMAPI_STREAMOPERATORS_H
