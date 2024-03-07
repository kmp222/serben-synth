#include "envelope.hpp"

envelopeADSR::envelopeADSR()
    : attack_time(0.01),
      decay_time(0.01),
      release_time(1.00),
      start_amplitude(1.0),
      sustain_amplitude(1.0) {}

envelopeADSR::envelopeADSR(double att, double dec, double rel, double start, double sus)
    : attack_time(att),
      decay_time(dec),
      release_time(rel),
      start_amplitude(start),
      sustain_amplitude(sus) {}

double envelopeADSR::get_amplitude(double time) {

    double amplitude = 0.0;
    double lifetime = 0.0;
    double release_amplitude = 0.0;

    if (note_is_on) {
        
        lifetime = time - trigger_on_time;

        // attack
        if (lifetime <= attack_time) {
            amplitude = (lifetime / attack_time) * start_amplitude;
        }

        // decay
        if (lifetime > attack_time && lifetime <= (attack_time + decay_time)) {
            amplitude = ((lifetime - attack_time) / decay_time) * 
                (sustain_amplitude - start_amplitude) + start_amplitude;
        }

        // sustain
        if (lifetime > (attack_time + decay_time)) {
            amplitude = sustain_amplitude;
        }

    } else {

        lifetime = trigger_off_time - time;

        // release
        if (lifetime <= attack_time)
            release_amplitude = (lifetime / attack_time) * start_amplitude;

        if (lifetime > attack_time && lifetime <= (attack_time + decay_time))
            release_amplitude = ((lifetime - attack_time) / decay_time) * (sustain_amplitude - start_amplitude) + start_amplitude;

        if (lifetime > (attack_time + decay_time))
            release_amplitude = sustain_amplitude;
        
        amplitude = ((time - trigger_off_time) / release_time) *
            (0.0 - release_amplitude) + release_amplitude;

    }

    if (amplitude <= 0.1) {
        amplitude = 0.0;
    }

    return amplitude;

}

void envelopeADSR::note_on(double time_on) {

    trigger_on_time = time_on;
    note_is_on = true;

}

void envelopeADSR::note_off(double time_off) {

    trigger_off_time = time_off;
    note_is_on = false;

}