//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_CORE_INCLUDE_IOSTREAM_H
#define DIGIT_RECOGNIZER_CORE_INCLUDE_IOSTREAM_H

#include <iostream>

using IStream = std::istream;
using OStream = std::ostream;
using IOStream = std::iostream;

inline constexpr auto &s_cout = std::cout;
inline constexpr auto &s_cerr = std::cerr;
inline constexpr auto &s_cin = std::cin;

template<typename T, typename std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
std::ostream &write(std::ostream &out, const T &object, size_t = 0) { return out << object; }

#endif //DIGIT_RECOGNIZER_CORE_INCLUDE_IOSTREAM_H
