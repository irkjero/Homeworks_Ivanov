//
// Created by Петр on 27.12.2020.
//

#ifndef STREAMAPI_STREAMTYPES_H
#define STREAMAPI_STREAMTYPES_H

#include <type_traits>


template<typename T> class Stream;

template<typename T> struct StreamIdentifier { using type = void; };
template<typename T> struct StreamIdentifier<Stream<T>> { using type = T; };


template<typename S> using StreamType =
typename StreamIdentifier<std::remove_reference_t<S>>::type;

#endif //STREAMAPI_STREAMTYPES_H
