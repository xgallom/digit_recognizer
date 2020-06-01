//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_CORE_INCLUDE_VECTOR_H
#define DIGIT_RECOGNIZER_CORE_INCLUDE_VECTOR_H

#include <vector>
#include "IOStream.h"

template<typename T>
using Vector = std::vector<T>;

template<typename T>
OStream &write(OStream &out, const Vector<T> &object, size_t offset = 0)
{
	out << "[\n";

	const std::string prefix(offset + 1, '\t');

	for(const auto &entry : object) {
		out << prefix;
		write(out, entry, offset + 1);
		out << ",\n";
	}

	return out << &prefix[1] << "]";
}

template<typename T>
OStream &operator<<(OStream &out, const Vector<T> &object) { return write(out, object); }

#endif //DIGIT_RECOGNIZER_CORE_INCLUDE_VECTOR_H
