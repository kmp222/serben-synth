#include <atomic>
#include "synth.hpp"

Synth::Synth()
    : master_volume(1.0),
      frequency(0.0),
      env(),
      osc1(),
      osc2(),
      osc3() {}

Synth::Synth(Preset p)
    : master_volume(1.0),
      frequency(0.0),
      env(p.attack, p.decay, p.release, p.start, p.sustain),
      osc1(p.w, p.lfo_hertz, p.lfo_amplitude),
      osc2(p.w2, p.lfo_hertz2, p.lfo_amplitude2),
      osc3(p.w3, p.lfo_hertz3, p.lfo_amplitude3),
      osc1_volume(p.o1_volume),
      osc2_volume(p.o2_volume),
      osc3_volume(p.o3_volume) {}

double Synth::master_sound(double time) {

    double sound = env.get_amplitude(time) *
        (

            + osc1_volume * osc1.sound(time, frequency)
            + osc2_volume * osc2.sound(time, frequency)
            + osc3_volume * osc3.sound(time, frequency)

        );

    return master_volume * sound;

}
