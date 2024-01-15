#pragma once

#include "utils.hpp"

struct Osc {

    Waves wave;
    double lfo_hertz = 0.0;
    double lfo_amplitude = 0.0;
    
    Osc();

    Osc(Waves w, double lfo_h, double lfo_a);

    double sound(double time, double frequency);

};
