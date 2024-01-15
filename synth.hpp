#pragma once

#include "envelope.hpp"
#include "oscillator.hpp"
#include "preset.hpp"

struct Synth {

    double master_volume;
    std::atomic<double> frequency;

    envelopeADSR env;

    Osc osc1;
    double osc1_volume;

    Osc osc2;
    double osc2_volume;

    Osc osc3;
    double osc3_volume;

    Synth();
    
    Synth(Preset p);

    double master_sound(double time);

};