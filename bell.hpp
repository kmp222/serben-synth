#pragma once
#include "instrument.hpp"
#include "oscillator.hpp"

struct bell : public instrument {

    bell();

    double sound(double time, double frequency) override;

};