#pragma once

// an envelope modifies an oscillator's amplitude by multiplying
// a second amplitude obtained through ADSR parameters w.r.t. time

struct envelopeADSR {

    double attack_time; // seconds it takes to reach start_amplitude
    double decay_time; // seconds it takes after the attack to settle to a constant volume (sustain_amplitude)
    double release_time; // seconds it takes to fade

    double start_amplitude; // volume after attack
    double sustain_amplitude; // sustain volume

    // these vars track the time of the note being pressed and released
    double trigger_on_time = 0.0;
    double trigger_off_time = 0.0;

    bool note_is_on = false;

    envelopeADSR();

    envelopeADSR(double att, double dec, double rel, double start, double sus);

    double get_amplitude(double time);

    // these functions help with managing the difference
    // between ADS and R (note pressed and note released)
    void note_on(double time_on);

    void note_off(double time_off);

};