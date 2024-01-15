#pragma once
#include "instrument.hpp"
#include "bell.hpp"

struct Synth {

    double master_volume;
    std::atomic<double> frequency;
    double octave_base_freq;
    double twelfth_root_of_two;
    instrument *voice;

    Synth();

};