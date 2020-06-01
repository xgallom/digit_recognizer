//
// Created by xgallom on 6/1/20.
//

#include <Activation.h>

#include <cmath>

namespace Activation {
	double Relu(double x)
	{
		return std::max(0.0, x);
	}
}
