#pragma once

// an oscillator is made of a basic wave and a LFO for adding textures

struct Osc {

    // list of all possible waves
    enum Waves {

        SINE = 0,
        SQUARE = 1,
        TRIANGLE = 2,
        ANALOG_SAW = 3,
        DIGITAL_SAW = 4,
        NOISE = 5

    };
    
    double volume = 1.0;
    Waves wave = SINE;
    double lfo_hertz = 0.0;
    double lfo_amplitude = 0.0;
    int transpose = 0;

    Osc(double v, Waves w, double lfo_h, double lfo_a, int t);

    // returns wave amplitude value w.r.t. time and frequency of note
    double sound(double time, double frequency);

};
