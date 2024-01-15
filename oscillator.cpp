#include <cmath>
#include "oscillator.hpp"

#define M_PI 3.14159265358979323846

Osc::Osc() {

    wave = SINE;
    lfo_hertz = 0;
    lfo_amplitude = 0;

}

Osc::Osc(Waves w, double lfo_h, double lfo_a) {

    wave = w;
    lfo_hertz = lfo_h;
    lfo_amplitude = lfo_a;

}

double Osc::sound(double time, double frequency) {

    double freq = w(frequency) * time + lfo_amplitude * frequency * sin(w(lfo_hertz) * time);

    switch(wave) {

        case SINE:
            return sin(freq);

        case SQUARE:
            return sin(freq) > 0.0 ? 1.0 : -1.0;
        
        case TRIANGLE:
            return asin( sin(freq) ) * (2.0 / M_PI);
        
        case ANALOG_SAW:
            {
                double output = 0.0;
                
                for (double i = 1.0; i < 50.0; ++i) {
                    output += ( sin(i * freq)) / i;
                }

                return output * (2.0 / M_PI);
            }
        
        case DIGITAL_SAW:
            return (frequency * M_PI * fmod(time, 1.0 / frequency)) * (2.0 / M_PI) - (M_PI / 2.0);

        case NOISE:
            return 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;

        default:
            return 0.0;

    }

}