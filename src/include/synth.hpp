#pragma once

#include "envelope.hpp"
#include "oscillator.hpp"
#include "preset.hpp"

// a synth is made of one envelope and multiple oscillators
// envelope and oscillators are set via sound preset

struct Synth {

    double master_volume = 2.0;
    double frequency = 0.0;

    // master envelope
    envelopeADSR env;

    // three oscillators
    Osc osc1;
    Osc osc2;
    Osc osc3;

    // default preset (simple sine wave)
    Synth();
    
    // custom preset
    Synth(Preset p);

    // return master sound with respect to time
    double master_sound(double time);

};