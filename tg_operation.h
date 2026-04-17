#ifndef TG_OPERATION_H
#define TG_OPERATION_H

#include "i_unary_operation.h"
#include <cmath>

class TgOperation final : public IUnaryOperation {
    double unaryExecute(double arg) const override {
        double radians = arg * (std::numbers::pi / 180.0);
        return std::abs(std::cos(radians)) < 1e-10 ? throw std::domain_error("Tangent is undefined at this point")
                                                   : std::tan(radians);
    }
};

#endif // TG_OPERATION_H
