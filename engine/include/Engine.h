//
// Created by xgallom on 5/31/20.
//

#ifndef DIGIT_RECOGNIZER_ENGINE_INCLUDE_ENGINE_H
#define DIGIT_RECOGNIZER_ENGINE_INCLUDE_ENGINE_H

#include "layer/Layer.h"

#include <Array.h>
#include <Vector.h>

#include <cstdint>

namespace Result {
	constexpr auto Digits = 10u;

	using CategoricalVectorType = Array<double, Digits>;

	template<uint8_t Digit>
	constexpr CategoricalVectorType CreateCategoricalVector()
	{
		static_assert(Digit < Digits, "Digit out of range");

		CategoricalVectorType result = {};

		result[Digit] = 1.0;

		return result;
	}

	constexpr Array<CategoricalVectorType, Digits> CategoricalVector = {
			CreateCategoricalVector<0>(),
			CreateCategoricalVector<1>(),
			CreateCategoricalVector<2>(),
			CreateCategoricalVector<3>(),
			CreateCategoricalVector<4>(),
			CreateCategoricalVector<5>(),
			CreateCategoricalVector<6>(),
			CreateCategoricalVector<7>(),
			CreateCategoricalVector<8>(),
			CreateCategoricalVector<9>(),
	};
}

class Engine {
public:
	template<typename T>
	void add(T &layer)
	{
		m_layers.emplace_back(ConnectArg<&T::execute>, &layer);
	}

private:
	Vector<Layer> m_layers = {};
};

#endif //DIGIT_RECOGNIZER_ENGINE_INCLUDE_ENGINE_H
