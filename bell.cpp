#include "bell.hpp"

bell::bell() {

    volume = 0.25;
    env.attack_time = 0.01;
    env.decay_time = 1.0;
    env.start_amplitude = 1.0;
    env.sustain_amplitude = 0.0;
    env.release_time = 1.0;

}

double bell::sound(double time, double frequency) {

    double s = env.get_amplitude(time) * 
        (

        + 1.0 * osc(frequency * 2.0, time, 0, 5.0, 0.001)
        + 0.5 * osc(frequency * 3.0, time, 0)
        + 0.25 * osc(frequency * 4.0, time, 0)

        );

    return s * volume;

}