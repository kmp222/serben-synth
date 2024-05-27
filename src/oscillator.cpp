#include <cmath>

#include "oscillator.hpp"
#include "utils.hpp"
#include <iostream>

#define M_PI 3.14159265358979323846

Osc::Osc(double v, Waves w, double lfo_h, double lfo_a, double t)
    : volume(v),
      wave(w),
      lfo_hertz(lfo_h),
      lfo_amplitude(lfo_a),
      transpose(t) {}

double Osc::sound(double time, double frequency) {
    
    frequency = frequency * pow(2, transpose / 12.0);

    double amp = w(frequency) * time + lfo_amplitude * frequency * sin(w(lfo_hertz) * time);
    double output;

    switch(wave) {

        case SINE:
            output = sin(amp);
            break;

        case SQUARE:
            output = sin(amp) > 0.0 ? 1.0 : -1.0;
            break;
        
        case TRIANGLE:
            output = asin( sin(amp) ) * (2.0 / M_PI);
            break;
        
        /* case ANALOG_SAW:
            {
                double output = 0.0;
                
                for (double i = 1.0; i < 50.0; ++i) {
                    output += ( sin(i * amp)) / i;
                }

                output = output * (2.0 / M_PI);
            }
            break; */
        
        case DIGITAL_SAW:
            output = (frequency * M_PI * fmod(time, 1.0 / frequency)) * (2.0 / M_PI) - (M_PI / 2.0);
            break;

        case NOISE:
            output = frequency != 0 ? 2.0 * ((double) rand() / (double) RAND_MAX) * amp - 1.0 : 0;
            break;

        /* case LASER:
            output = (amp * M_PI * fmod(time, 1.0 / amp)) * (2.0 / M_PI) - (M_PI / 2.0)
            break;
        */

        default:
            output = 0.0;

    }

    return output * volume;
    
}