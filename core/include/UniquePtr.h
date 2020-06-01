//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_CORE_INCLUDE_UNIQUEPTR_H
#define DIGIT_RECOGNIZER_CORE_INCLUDE_UNIQUEPTR_H

#include <memory>

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
constexpr auto makeUnique = std::make_unique<T>;

#endif //DIGIT_RECOGNIZER_CORE_INCLUDE_UNIQUEPTR_H
