//
// Created by Петр on 27.12.2020.
//

#ifndef STREAMAPI_STREAMTERMINATORS_H
#define STREAMAPI_STREAMTERMINATORS_H

#include <iostream>
#include "StreamOperations.h"
#include "Streamexcept.h"
#include "StreamOperators.h"

template<typename Function>
inline auto for_each(Function&& function) {
    return make_terminator("stream::op::for_each", [=](auto&& stream) mutable {
        auto& source = stream.getSource();
        while(source->advance()) {
            function(std::move(*source->get()));
        }
        return function;
    });
}

template<typename U, typename Accumulator>
inline auto identity_reduce(const U& identity, Accumulator&& accumulator) {
    return make_terminator("stream::op::identity_reduce", [=](auto&& stream) mutable {
        auto& source = stream.getSource();
        U result = identity;
        while(source->advance()) {
            result = accumulator(std::move(result), std::move(*source->get()));
        }
        return result;
    });
}

template<typename Accumulator>
inline auto reduce(Accumulator&& accumulator) {
    return make_terminator("stream::op::reduce", [=](auto&& stream) mutable {
        auto& source = stream.getSource();
        if(source->advance()) {
            auto reduction = identity_reduce(std::move(*source->get()),
                                             std::forward<Accumulator>(accumulator));
            return stream | reduction;
        } else {
            throw EmptyStreamException("stream::op::reduce");
        }
    });
}

template<typename IdentityFn, typename Accumulator>
inline auto reduce(IdentityFn&& identityFn, Accumulator&& accumulator) {
    return make_terminator("stream::op::reduce", [=](auto&& stream) mutable {
        auto& source = stream.getSource();
        if(source->advance()) {
            auto reduction = identity_reduce(identityFn(std::move(*source->get())),
                                             std::forward<Accumulator>(accumulator));
            return stream | reduction;
        } else {
            throw EmptyStreamException("stream::op::reduce");
        }
    });
}

inline auto first() {
    return make_terminator("stream::op::first", [=](auto&& stream) {
        auto& source = stream.getSource();
        if(source->advance()) {
            return std::move(*source->get());
        } else {
            throw EmptyStreamException("stream::op::first");
        }
    });
}

inline auto last() {
    return reduce([](auto&& first, auto&& second) { return second; })
            .rename("stream::op::last");
}

inline auto nth(size_t index) {
    return (skip(index) | first()).rename("stream::op::nth");
}

inline auto sum() {
    return reduce(std::plus<void>())
            .rename("stream::op::sum");
}

template<typename T>
inline auto sum(const T& identity) {
    return identity_reduce(identity, std::plus<T>())
            .rename("stream::op::sum");
}


template<typename OutputIterator>
inline auto copy_to(OutputIterator out) {
    return make_terminator("stream::op::copy_to", [=](auto&& stream) mutable {
        using T = StreamType<decltype(stream)>;
        auto& source = stream.getSource();
        while(source->advance()) {
            *out = *source->get();
            //std::cout << *source->get() << std::endl;
            ++out;
            ++out;
        }
        return out;
    });
}

template<typename OutputIterator>
inline auto move_to(OutputIterator out) {
    return make_terminator("stream::op::move_to", [=](auto&& stream) mutable {
        using T = StreamType<decltype(stream)>;
        auto& source = stream.getSource();
        while(source->advance()) {
            *out = std::move(*source->get());
            ++out;
        }
        return out;
    });
}

inline auto print_to(std::ostream& os, const char* delimiter = " ") {
    return make_terminator("stream::op::print_to", [&os, delimiter](auto&& stream) -> std::ostream& {
        using T = StreamType<decltype(stream)>;
        stream | copy_to(std::ostream_iterator<T>(os, delimiter));
        return os;
    });
}

#endif //STREAMAPI_STREAMTERMINATORS_H
