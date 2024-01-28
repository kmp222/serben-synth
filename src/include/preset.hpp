#pragma once

#include <string>

#include "oscillator.hpp"

// a preset is a structure data that contains parameters to set the synth

struct Preset {

        // info
        std::string name;

        // envelope parameters
        double attack;
        double decay;
        double release;
        double start;
        double sustain;

        // osc1 param
        Osc::Waves wave;
        double o_volume;
        double lfo_hertz;
        double lfo_amplitude;

        // osc2 param
        Osc::Waves wave2;
        double o2_volume;
        double lfo_hertz2;
        double lfo_amplitude2;

        // osc3 param
        Osc::Waves wave3;
        double o3_volume;
        double lfo_hertz3;
        double lfo_amplitude3;

        Preset(std::string n, double a, double d, double r, double st, double s,
               Osc::Waves w, double o_vol, double lfo_hz, double lfo_amp,
               Osc::Waves w2, double o_vol2, double lfo_hz2, double lfo_amp2,
               Osc::Waves w3, double o_vol3, double lfo_hz3, double lfo_amp3);

};
