//
// Created by Петр on 27.12.2020.
//

#ifndef STREAMAPI_UTILS_H
#define STREAMAPI_UTILS_H

#include "StreamProvider.h"
using namespace provider;

struct IncompleteGroupError {};

template<typename T>
class DynamicGroup : public StreamProvider<std::vector<T>> {

public:
DynamicGroup(StreamProviderPtr<T> source, size_t N) : source_(std::move(source)), N_(N) {}

std::shared_ptr<std::vector<T>> get() override {
return current_;
}

bool advance_impl() override {
current_ = std::make_shared<std::vector<T>>();
current_->reserve(N_);
for(int i = 0; i < N_; i++) {
if(source_->advance()) {
current_->emplace_back(std::move(*source_->get()));
} else {
current_.reset();
return false;
}
}
return true;
}

PrintInfo print(std::ostream& os, int indent) const override {
this->print_indent(os, indent);
os << "Grouped[" << N_ << "]:\n";
return source_->print(os, indent + 1).addStage();
}

private:
StreamProviderPtr<T> source_;
std::shared_ptr<std::vector<T>> current_;
const size_t N_;

};


template<typename T, typename Predicate>
class Filter : public StreamProvider<T> {

public:
    Filter(StreamProviderPtr<T> source, Predicate&& predicate)
            : source_(std::move(source)), predicate_(predicate) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance_impl() override {
        while(source_->advance()) {
            current_ = source_->get();
            if(predicate_(*current_)) {
                return true;
            }
        }
        current_.reset();
        return false;
    }

    PrintInfo print(std::ostream& os, int indent) const override {
        this->print_indent(os, indent);
        os << "Filter:\n";
        return source_->print(os, indent + 1).addStage();
    }

private:
    StreamProviderPtr<T> source_;
    Predicate predicate_;
    std::shared_ptr<T> current_;
};

template<typename T>
class Slice : public StreamProvider<T> {

public:
    Slice(StreamProviderPtr<T> source, size_t start, size_t end, size_t increment, bool no_end_)
            : source_(std::move(source)),
              start_(start),
              end_(end),
              increment_(increment),
              no_end_(no_end_) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance_impl() override {
        if(index_ < start_) {
            for(; index_ <= start_; index_++) {
                if(source_->advance()) {
                    current_ = source_->get();
                } else {
                    current_.reset();
                    return false;
                }
            }
            return true;
        }

        if(no_end_ || index_ + increment_ <= end_) {
            for(size_t k = 0; k < increment_; k++) {
                index_++;
                if(source_->advance()) {
                    current_ = source_->get();
                } else {
                    current_.reset();
                    return false;
                }
            }
            return true;
        }

        current_.reset();
        return false;
    }

    PrintInfo print(std::ostream& os, int indent) const override {
        this->print_indent(os, indent);
        os << "Slice[" << start_ << ", "
           << (no_end_ ? -1 : static_cast<long>(end_)) << ", "
           << increment_ <<  "]:\n";
        return source_->print(os, indent + 1).addStage();
    }

private:
    bool first_ = true;
    StreamProviderPtr<T> source_;
    std::shared_ptr<T> current_;
    size_t index_ = 0;
    size_t start_;
    size_t end_;
    size_t increment_;
    bool no_end_;


};

template<typename T, typename Transform, typename In>
class Map : public StreamProvider<T> {

public:
    Map(StreamProviderPtr<In> source, Transform&& transform)
            : source_(std::move(source)), transform_(transform) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance_impl() override {
        if(source_->advance()) {
            current_ = std::make_shared<T>(transform_(std::move(*source_->get())));
            return true;
        }
        current_.reset();
        return false;
    }

    PrintInfo print(std::ostream& os, int indent) const override {
        this->print_indent(os, indent);
        os << "Map:\n";
        return source_->print(os, indent + 1).addStage();
    }

private:
    StreamProviderPtr<In> source_;
    Transform transform_;
    std::shared_ptr<T> current_;

};


template<typename A, typename B>
struct Append {
};

template<template<typename...> class Base, typename... Args, typename Head>
struct Append<Head, Base<Args...>> {
    using Type = Base<Head, Args...>;
};

template<typename T, size_t N>
struct NTupleImpl {
    using SubType = typename NTupleImpl<T, N-1>::Type;
    using Type = typename Append<T, SubType>::Type;
};

template<typename T>
struct NTupleImpl<T, 1> {
    using Type = std::tuple<T>;
};

template<typename T, size_t N>
using NTuple = typename NTupleImpl<T, N>::Type;

template<typename T>
std::shared_ptr<T> next(StreamProviderPtr<T>& source) {
    if(source->advance()) {
        return source->get();
    }
    throw IncompleteGroupError();
}

template<typename T, size_t N>
struct Grouper {
    using Type = NTuple<T, N>;

    static Type group(StreamProviderPtr<T>& source) {
        auto sub = Grouper<T, N-1>::group(source);
        auto curr = next(source);
        return std::tuple_cat(sub, std::make_tuple<T>(std::move(*curr)));
    }
};

template<typename T>
struct Grouper<T, 3> {
    using Type = NTuple<T, 3>;

    static Type group(StreamProviderPtr<T>& source) {
        auto first = next(source);
        auto second = next(source);
        auto third = next(source);
        return std::make_tuple<T, T, T>(std::move(*first),
                                        std::move(*second),
                                        std::move(*third));
    }
};

template<typename T>
struct Grouper<T, 2> {
    using Type = std::pair<T, T>;

    static Type group(StreamProviderPtr<T>& source) {
        auto first = next(source);
        auto second = next(source);
        return std::make_pair<T, T>(std::move(*first),
                                    std::move(*second));
    }
};

template<typename T, size_t N>
using GroupResult = typename Grouper<T, N>::Type;


template<typename T, size_t N>
class Group : public StreamProvider<GroupResult<T, N>> {
static_assert(N >= 2, "Cannot group stream into chunks of less than size 2.");

public:
using GroupType = GroupResult<T, N>;

Group(StreamProviderPtr<T> source) : source_(std::move(source)) {}

std::shared_ptr<GroupType> get() override {
return current_;
}

bool advance_impl() override {
try {
auto group = Grouper<T, N>::group(source_);
current_ = std::make_shared<GroupType>(std::move(group));
return true;
} catch(IncompleteGroupError& err) {
return false;
}
}

PrintInfo print(std::ostream& os, int indent) const override {
this->print_indent(os, indent);
os << "Grouped[" << N << "]:\n";
return source_->print(os, indent + 1).addStage();
}

private:
StreamProviderPtr<T> source_;
std::shared_ptr<GroupType> current_;

};


template<typename T>
class Empty : public StreamProvider<T> {

public:
    std::shared_ptr<T> get() override {
        return nullptr;
    }

    bool advance_impl() override {
        return false;
    }

    PrintInfo print(std::ostream& os, int indent) const override {
        this->print_indent(os, indent);
        os << "[empty stream]\n";
        return PrintInfo::Source();
    }

};


#endif //STREAMAPI_UTILS_H
