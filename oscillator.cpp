#include <cmath>

#include "oscillator.hpp"
#include "utils.hpp"

#define M_PI 3.14159265358979323846

Osc::Osc(Waves w, double lfo_h, double lfo_a)
    : wave(w),
      lfo_hertz(lfo_h),
      lfo_amplitude(lfo_a) {}

double Osc::sound(double time, double frequency) {

    double amp = w(frequency) * time + lfo_amplitude * frequency * sin(w(lfo_hertz) * time);

    switch(wave) {

        case SINE:
            return sin(amp);

        case SQUARE:
            return sin(amp) > 0.0 ? 1.0 : -1.0;
        
        case TRIANGLE:
            return asin( sin(amp) ) * (2.0 / M_PI);
        
        case ANALOG_SAW:
            {
                double output = 0.0;
                
                for (double i = 1.0; i < 50.0; ++i) {
                    output += ( sin(i * amp)) / i;
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