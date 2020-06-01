//
// Created by xgallom on 6/1/20.
//

#ifndef DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_LAYER_H
#define DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_LAYER_H

#include <Delegate.h>
#include <Vector.h>

using LayerExecuteFunction = void (Vector<double> &, const Vector<double> &, size_t &, size_t &);
using Layer = Delegate<LayerExecuteFunction>;

#endif //DIGIT_RECOGNIZER_ENGINE_INCLUDE_LAYER_LAYER_H
