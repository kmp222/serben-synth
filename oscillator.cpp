#include <cmath>
#include "oscillator.hpp"

#define M_PI 3.14159265358979323846

double w(double hertz) {
    return hertz * 2.0 * M_PI;
}

double osc(double hertz, double time, int type, double lfo_hertz, double lfo_amplitude) {

    double freq = w(hertz) * time + lfo_amplitude * hertz * sin(w(lfo_hertz) * time);

    switch(type) {

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
            return (hertz * M_PI * fmod(time, 1.0 / hertz)) * (2.0 / M_PI) - (M_PI / 2.0);

        case NOISE:
            return 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;

        default:
            return 0.0;

    }

}