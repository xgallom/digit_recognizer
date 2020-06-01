//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_CONVOLUTION_H
#define DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_CONVOLUTION_H

#include "../Activation.h"

#include <Array.h>
#include <Vector.h>

struct Convolution {
	size_t filters;
	Array<size_t, 2u> kernelSize;
	Activation::Function &activation;

	void execute(Vector<double> &output, const Vector<double> &input, size_t &width, size_t &height) noexcept;
};

#endif //DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_CONVOLUTION_H
