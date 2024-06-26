#include "synth.hpp"
#include <math.h>

Synth::Synth()
    : env(0.01, 0.01, 1.0, 1.0, 1.0),
      osc1(1.0, Osc::SINE, 0.0, 0.0, 0),
      osc2(0.5, Osc::SINE, 0.0, 0.0, 0),
      osc3(0.5, Osc::SINE, 0.0, 0.0, 0)
      {}

// sound = envelope * (sum of waves) w.r.t. time
// master_volume reduces the overall volume resulting from the sum
double Synth::master_sound(double time) {

    double sound = env.get_amplitude(time) *
        (

            +osc1.sound(time, frequency)
            +osc2.sound(time, frequency)
            +osc3.sound(time, frequency)
    
        );
    
    return sound * master_volume;

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

Preset Synth::save_preset() {
    
    Preset p(

        env.attack_time, env.decay_time, env.release_time, env.start_amplitude, env.sustain_amplitude,
        osc1.wave, osc1.volume, osc1.lfo_hertz, osc1.lfo_amplitude, osc1.transpose,
        osc2.wave, osc2.volume, osc2.lfo_hertz, osc2.lfo_amplitude, osc2.transpose,
        osc3.wave, osc3.volume, osc3.lfo_hertz, osc3.lfo_amplitude, osc3.transpose
    
        );

    return p;

}
