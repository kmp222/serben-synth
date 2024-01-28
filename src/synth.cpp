#include "synth.hpp"
#include "utils.hpp"

Synth::Synth()
    : env(0.01, 0.01, 0.01, 1.0, 1.0),
      osc1(1.0, Waves::SINE, 0.0, 0.0),
      osc2(0.0, Waves::SINE, 0.0, 0.0),
      osc3(0.0, Waves::SINE, 0.0, 0.0)
      {}

Synth::Synth(Preset p)
    : env(p.attack, p.decay, p.release, p.start, p.sustain),
      osc1(p.o_volume, p.wave, p.lfo_hertz, p.lfo_amplitude),
      osc2(p.o2_volume, p.wave2, p.lfo_hertz2, p.lfo_amplitude2),
      osc3(p.o3_volume, p.wave3, p.lfo_hertz3, p.lfo_amplitude3)
      {}

// sound = envelope * (sum of waves) w.r.t. time
// master_volume reduces the overall volume resulting from the sum
double Synth::master_sound(double time) {

    double master;

    double sound = env.get_amplitude(time) *
        (

            + osc1.sound(time, frequency)
            + osc2.sound(time, frequency)
            + osc3.sound(time, frequency)
    
        );

    master = sound * master_volume;
    
    return master;

}
