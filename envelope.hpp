#pragma once

struct envelopeADSR {

    double attack_time;
    double decay_time;
    double release_time;

    double start_amplitude;
    double sustain_amplitude;

    double trigger_on_time;
    double trigger_off_time;

    bool note_is_on;

    envelopeADSR();

    double get_amplitude(double time);

    void note_on(double time_on);

    void note_off(double time_off);

};