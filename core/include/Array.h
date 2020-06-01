//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_CORE_INCLUDE_ARRAY_H
#define DIGIT_RECOGNIZER_CORE_INCLUDE_ARRAY_H

#include "IOStream.h"
#include <array>

template<typename T, size_t N>
using Array = std::array<T, N>;

template<typename T, size_t N>
inline OStream &write(OStream &out, const Array<T, N> &object, size_t offset = 0)
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

template<typename T, size_t N>
inline OStream &operator<<(OStream &out, const Array<T, N> &object) { return write(out, object); }

#endif //DIGIT_RECOGNIZER_CORE_INCLUDE_ARRAY_H
