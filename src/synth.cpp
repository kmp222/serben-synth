#include "synth.hpp"

Synth::Synth()
    : env(0.01, 0.01, 0.01, 1.0, 1.0),
      osc1(1.0, Osc::SINE, 0.0, 0.0),
      osc2(0.0, Osc::SINE, 0.0, 0.0),
      osc3(0.0, Osc::SINE, 0.0, 0.0)
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

void Synth::load_preset(Preset p) {

    env.attack_time = p.attack;
    env.decay_time = p.decay;
    env.release_time = p.release;
    env.start_amplitude = p.start;
    env.sustain_amplitude = p.sustain;

    osc1.volume = p.o_volume;
    osc1.wave = p.wave;
    osc1.lfo_hertz = p.lfo_hertz;
    osc1.lfo_amplitude = p.lfo_amplitude;

    osc2.volume = p.o2_volume;
    osc2.wave = p.wave2;
    osc2.lfo_hertz = p.lfo_hertz2;
    osc2.lfo_amplitude = p.lfo_amplitude2;

    osc3.volume = p.o3_volume;
    osc3.wave = p.wave3;
    osc3.lfo_hertz = p.lfo_hertz3;
    osc3.lfo_amplitude = p.lfo_amplitude3;    

}
