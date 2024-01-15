#pragma once

struct envelopeADSR {

    double attack_time;
    double decay_time;
    double release_time;

    double start_amplitude;
    double sustain_amplitude;

    double trigger_on_time = 0.0;
    double trigger_off_time = 0.0;

    bool note_is_on = false;

    envelopeADSR();

    envelopeADSR(double att, double dec, double rel, double start, double sus);

    double get_amplitude(double time);

    void note_on(double time_on);

    void note_off(double time_off);

};