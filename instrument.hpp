#pragma once
#include "envelope.hpp"

struct instrument {

    double volume;
    envelopeADSR env;
    virtual ~instrument() {};

    virtual double sound(double time, double frequency) = 0;

};