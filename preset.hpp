#pragma once

#include <string>

#include "oscillator.hpp"
#include "utils.hpp"

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
        Waves wave;
        double o1_volume;
        double lfo_hertz;
        double lfo_amplitude;

        // osc2 param
        Waves wave2;
        double o2_volume;
        double lfo_hertz2;
        double lfo_amplitude2;

        // osc3 param
        Waves wave3;
        double o3_volume;
        double lfo_hertz3;
        double lfo_amplitude3;

        Preset(std::string n, double a, double d, double r, double st, double s,
               Waves w, double o_vol, double lfo_hz, double lfo_amp,
               Waves w2, double o_vol2, double lfo_hz2, double lfo_amp2,
               Waves w3, double o_vol3, double lfo_hz3, double lfo_amp3)
        : name(n), attack(a), decay(d), release(r), start(st), sustain(s),
          wave(w), o1_volume(o_vol), lfo_hertz(lfo_hz), lfo_amplitude(lfo_amp),
          wave2(w2), o2_volume(o_vol2), lfo_hertz2(lfo_hz2), lfo_amplitude2(lfo_amp2),
          wave3(w3), o3_volume(o_vol3), lfo_hertz3(lfo_hz3), lfo_amplitude3(lfo_amp3) {}

};
