#pragma once

#include <string>

#include "oscillator.hpp"

// a preset is a structure data that contains parameters to set the synth

struct Preset {

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
        int transpose;

        // osc2 param
        Osc::Waves wave2;
        double o2_volume;
        double lfo_hertz2;
        double lfo_amplitude2;
        int transpose2;

        // osc3 param
        Osc::Waves wave3;
        double o3_volume;
        double lfo_hertz3;
        double lfo_amplitude3;
        int transpose3;

        Preset()
        : attack(0.0), decay(0.0), release(0.0), start(0.0), sustain(0.0),
          wave(Osc::Waves::SINE), o_volume(0.0), lfo_hertz(0.0), lfo_amplitude(0.0), transpose(0),
          wave2(Osc::Waves::SINE), o2_volume(0.0), lfo_hertz2(0.0), lfo_amplitude2(0.0), transpose2(0),
          wave3(Osc::Waves::SINE), o3_volume(0.0), lfo_hertz3(0.0), lfo_amplitude3(0.0), transpose3(0) {}
        
        Preset(double a, double d, double r, double st, double s,
               Osc::Waves w, double o_vol, double lfo_hz, double lfo_amp, int transpose,
               Osc::Waves w2, double o_vol2, double lfo_hz2, double lfo_amp2, int transpose2,
               Osc::Waves w3, double o_vol3, double lfo_hz3, double lfo_amp3, int transpose3);

};
