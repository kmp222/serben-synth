#include <string>

#include "preset.hpp"

Preset::Preset(std::string n, double a, double d, double r, double st, double s,
               Osc::Waves w, double o_vol, double lfo_hz, double lfo_amp,
               Osc::Waves w2, double o_vol2, double lfo_hz2, double lfo_amp2,
               Osc::Waves w3, double o_vol3, double lfo_hz3, double lfo_amp3)
: name(n), attack(a), decay(d), release(r), start(st), sustain(s),
  wave(w), o_volume(o_vol), lfo_hertz(lfo_hz), lfo_amplitude(lfo_amp),
  wave2(w2), o2_volume(o_vol2), lfo_hertz2(lfo_hz2), lfo_amplitude2(lfo_amp2),
  wave3(w3), o3_volume(o_vol3), lfo_hertz3(lfo_hz3), lfo_amplitude3(lfo_amp3) {}