#include <atomic>
#include <cmath>
#include "synth.hpp"

Synth::Synth() :
    master_volume(1.0),
    frequency(0.0),
    octave_base_freq(220), // A3
    twelfth_root_of_two(pow(2.0, 1.0 / 12.0)),
    voice(new bell())
{}


