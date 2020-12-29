//
// Created by Петр on 27.12.2020.
//

#ifndef STREAMAPI_STREAM_H
#define STREAMAPI_STREAM_H

#include "StreamProvider.h"
#include "Streamexcept.h"
#include "Utils.h"

#include <unordered_set>
#include <functional>
#include <type_traits>
#include <iostream>
#include <iterator>
#include <vector>
#include <random>
#include <chrono>
#include <deque>
#include <list>
#include <set>

using namespace provider;
template<typename T> class Operator;
template<typename T> class Terminator;

template<typename T>
class Stream {

public:
    using iterator = typename StreamProvider<T>::Iterator;

    iterator begin() {
        return source_->begin();
    }

    iterator end() {
        return source_->end();
    }

    Stream()
            : source_(make_stream_provider<Empty, T>()) {}

    Stream(Stream<T>&& other) = default;
    Stream<T>& operator= (Stream<T>&& other) = default;

    Stream(const Stream<T>& other) = delete;
    Stream<T>& operator= (const Stream<T>& other) = default;

    Stream(StreamProviderPtr<T> source)
            : source_(std::move(source)) {}

    template<typename Iterator>
    Stream(Iterator begin, Iterator end)
            : source_(make_stream_provider<provider::Iterator, T, Iterator>(
            begin, end)) {}

    template<typename Container>
    Stream(const Container& container)
            : Stream(container.begin(), container.end()) {
    }

    Stream(std::initializer_list<T> init)
            : Stream(std::deque<T>(init.begin(), init.end())) {
    }

    template<typename F>
    auto operator| (Operator<F>&& op) ->
    decltype(op.apply_to(std::move(*this))) {
        return op.apply_to(std::move(*this));
    }

    template<typename F>
    auto operator| (Operator<F>& op) ->
    decltype(op.apply_to(std::move(*this))) {
        return op.apply_to(std::move(*this));
    }

    template<typename F>
    auto operator| (Terminator<F>&& term) ->
    decltype(term.apply_to(std::move(*this))) {
        return term.apply_to(std::move(*this));
    }

    template<typename F>
    auto operator| (Terminator<F>& term) ->
    decltype(term.apply_to(std::move(*this))) {
        return term.apply_to(std::move(*this));
    }

    StreamProviderPtr<T>& getSource() {
        return source_;
    }

    void close() {
        source_.reset();
    }

    bool occupied() const {
        return bool(source_);
    }

    void swap(Stream<T>& other) {
        source_.swap(other.sorce_);
    }

    std::string pipeline() {
        std::stringstream ss;
        PrintInfo info = source_->print(ss, 1);
        ss << "Stream pipeline with "
           << info.stages << " stage" << (info.stages == 1 ? "" : "s") << " and "
           << info.sources << " source" << (info.sources == 1 ? "" : "s") << ".";
        return ss.str();
    }

    template<typename> friend class Operator;
    template<typename> friend class Terminator;

private:
    StreamProviderPtr<T> source_;

    void check_vacant(const std::string& method) {
        if(!occupied()) {
            throw VacantStreamException(method);
        }
    }

};


#endif //STREAMAPI_STREAM_H
