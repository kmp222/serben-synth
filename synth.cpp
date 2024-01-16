#include "synth.hpp"
#include "utils.hpp"

Synth::Synth()
    : env(0.01, 0.01, 0.01, 1.0, 1.0),
      osc1(Waves::SINE, 0.0, 0.0),
      osc2(Waves::SINE, 0.0, 0.0),
      osc3(Waves::SINE, 0.0, 0.0),
      osc1_volume(1.0),
      osc2_volume(0.0),
      osc3_volume(0.0) {}

Synth::Synth(Preset p)
    : env(p.attack, p.decay, p.release, p.start, p.sustain),
      osc1(p.wave, p.lfo_hertz, p.lfo_amplitude),
      osc2(p.wave2, p.lfo_hertz2, p.lfo_amplitude2),
      osc3(p.wave3, p.lfo_hertz3, p.lfo_amplitude3),
      osc1_volume(p.o1_volume),
      osc2_volume(p.o2_volume),
      osc3_volume(p.o3_volume) {}

// sound = envelope * (sum of waves) w.r.t. time
// master_volume reduces the overall volume resulting from the sum
double Synth::master_sound(double time) {

    double sound = env.get_amplitude(time) *
        (

            + osc1_volume * osc1.sound(time, frequency)
            + osc2_volume * osc2.sound(time, frequency)
            + osc3_volume * osc3.sound(time, frequency)

        );

    return master_volume * sound;

}
