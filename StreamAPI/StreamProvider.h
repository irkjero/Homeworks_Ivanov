//
// Created by Петр on 27.12.2020.
//

#ifndef STREAMAPI_STREAMPROVIDER_H
#define STREAMAPI_STREAMPROVIDER_H

#include "Streamexcept.h"

#include <memory>


struct PrintInfo {
    PrintInfo(int sources_, int stages_)
            : sources(sources_), stages(stages_) {}

    static PrintInfo Source() {
        return {1, 0};
    }

    PrintInfo operator+ (const PrintInfo& info) {
        return {sources + info.sources, stages + info.stages};
    }

    PrintInfo addStage(int stages = 1) {
        return {sources, this->stages + stages};
    }

    int sources;
    int stages;
};

namespace provider {

    template<typename T> struct StreamProvider;

    template<typename T, typename Itr>
    class Iterator : public StreamProvider<T> {

    public:
        Iterator(Itr begin, Itr end)
                : current_(begin), end_(end) {
        }

        std::shared_ptr<T> get() override {
            return std::make_shared<T>(std::move(*current_));
        }

        bool advance_impl() override {
            if(first_) {
                first_ = false;
                return current_ != end_;
            }
            ++current_;
            return current_ != end_;
        }

        PrintInfo print(std::ostream& os, int indent) const override {
            this->print_indent(os, indent);
            os << "[iterator stream]\n";
            return PrintInfo::Source();
        }

    private:
        bool first_ = true;
        Itr current_;
        Itr end_;

    };

} /* namespace provider */


namespace provider {

    template<typename T>
    struct StreamProvider {

    public:
        struct Iterator;

        virtual std::shared_ptr<T> get() = 0;

        bool advance() {
            try {
                return advance_impl();
            } catch (StopStream &stop) {
                return false;
            } catch (...) {
                throw;
            }
        }

        Iterator begin();

        Iterator end();

        virtual PrintInfo print(std::ostream &os, int indent) const = 0;

    protected:
        virtual bool advance_impl() = 0;

    protected:
        static void print_indent(std::ostream &os, int indent) {
            for (int i = 0; i < indent - 1; i++) {
                os << "  ";
            }
            if (indent != 0) {
                os << "> ";
            }
        }
    };


    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::begin() {
        return Iterator::begin(this);
    }

    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::end() {
        return Iterator::end(this);
    }


    template<typename T>
    using StreamProviderPtr = std::unique_ptr<StreamProvider<T>>;

    template<template<typename...> class Provider,
            typename T,
            typename... TemplateArgs,
            typename... ConstructorArgs>
    StreamProviderPtr<T> make_stream_provider(ConstructorArgs &&... args) {
        return StreamProviderPtr<T>(
                new Provider<T, TemplateArgs...>(
                        std::forward<ConstructorArgs>(args)...));
    }

    template<typename T>
    struct StreamProvider<T>::Iterator {

    public:
        T &operator*();

        Iterator &operator++();

        Iterator operator++(int);

        bool operator==(Iterator &other);

        bool operator!=(Iterator &other);

        friend struct StreamProvider<T>;

    private:
        enum class State {
            Initial,
            Consumed,
            Iterating,
            End
        };

        static Iterator begin(StreamProvider<T> *source);

        static Iterator iterating(StreamProvider<T> *source, std::shared_ptr<T> value);

        static Iterator end(StreamProvider<T> *source);

        Iterator(StreamProvider<T> *source, State state, std::shared_ptr<T> value);

        void update_initial();

        void check_consumed(const std::string &op) const;

        StreamProvider<T> *source_;
        State state_;
        std::shared_ptr<T> current_;

    };

    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::Iterator::begin(StreamProvider<T> *source) {
        return Iterator(source, State::Initial, nullptr);
    }

    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::Iterator::iterating(StreamProvider<T> *source,
                                           std::shared_ptr<T> value) {
        return Iterator(source, State::Iterating, value);
    }

    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::Iterator::end(StreamProvider<T> *source) {
        return Iterator(source, State::End, nullptr);
    }

    template<typename T>
    StreamProvider<T>::Iterator::Iterator(StreamProvider<T> *source, State state,
                                          std::shared_ptr<T> value)
            : source_(source), state_(state), current_(value) {}

    template<typename T>
    T &StreamProvider<T>::Iterator::operator*() {
        update_initial();
        return *current_;
    }

    template<typename T>
    typename StreamProvider<T>::Iterator &
    StreamProvider<T>::Iterator::operator++() {
        update_initial();
        check_consumed("prefix increment");
        if (source_->advance()) {
            current_ = source_->get();
        } else {
            state_ = State::End;
            current_.reset();
        }
        return *this;
    }

    template<typename T>
    typename StreamProvider<T>::Iterator
    StreamProvider<T>::Iterator::operator++(int) {
        update_initial();
        check_consumed("postfix increment");
        Iterator result(nullptr, State::Consumed, current_);
        if (source_->advance()) {
            current_ = source_->get();
        } else {
            state_ = State::End;
            current_.reset();
        }
        return result;
    }

    template<typename T>
    bool StreamProvider<T>::Iterator::operator==(Iterator &other) {
        update_initial();
        check_consumed("equality check");
        other.update_initial();
        other.check_consumed("equality check");
        if (state_ == State::End) {
            return other.state_ == State::End;
        }
        if (other.state_ == State::End) {
            return false;
        }
        return source_ == other.source_ && current_ == other.current_;
    }

    template<typename T>
    bool StreamProvider<T>::Iterator::operator!=(Iterator &other) {
        update_initial();
        check_consumed("inequality check");
        other.update_initial();
        other.check_consumed("inequality check");
        if (state_ == State::End) {
            return other.state_ != State::End;
        }
        if (other.state_ == State::End) {
            return true;
        }
        return source_ != other.source_ || current_ != other.current_;
    }

    template<typename T>
    void StreamProvider<T>::Iterator::update_initial() {
        if (state_ == State::Initial) {
            if (source_->advance()) {
                current_ = source_->get();
                state_ = State::Iterating;
            } else {
                current_.reset();
                state_ = State::End;
            }
        }
    }

    template<typename T>
    void StreamProvider<T>::Iterator::check_consumed(const std::string &op) const {
        if (state_ == State::Consumed) {
            throw ConsumedIteratorException(op);
        }
    }

}

#endif //STREAMAPI_STREAMPROVIDER_H
