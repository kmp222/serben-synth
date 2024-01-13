#include <iostream>
#include "olcNoiseMaker.h"

double frequency = 0.0;
double octave_base_freq = 110; // A2
double d12th_root_of_2 = pow(2.0, 1.0 / 12.0);

void print_keyboard() {

std::cout << "________________________________" << std::endl;
std::cout << "|  | |  |  | | | |  |  | | | |  |" << std::endl;
std::cout << "|  | |  |  | | | |  |  | | | |  |" << std::endl;
std::cout << "|  |S|  |  |F| |G|  |  |J| |K|  |" << std::endl;
std::cout << "|  |_|  |  |_| |_|  |  |_| |_|  |" << std::endl;
std::cout << "|   |   |   |   |   |   |   |   |" << std::endl;
std::cout << "| Z | X | C | V | B | N | M | , |" << std::endl;
std::cout << "|___|___|___|___|___|___|___|___|" << std::endl;

}

double w(double hertz) {
    return hertz * 2.0 * PI;
}

double osc(double hertz, double time, int type) {

    switch(type) {

        case 0: // sine
            return sin(w(hertz) * time);

        case 1: // square
            return sin(w(hertz) * time) > 0.0 ? 1.0 : -1.0;
        
        case 2: // triangle
            return asin( sin(w(hertz) * time) ) * (2.0 / PI);
        
        case 3: // saw (analog, warm)
            {
                double output = 0.0;
                
                for (double i = 1.0; i < 100.0; ++i) {
                    output += ( sin(i * w(hertz) * time)) / i;
                }

                return output * (2.0 / PI);
            }
        
        case 4: // saw (harsh)
            return (hertz * PI * fmod(time, 1.0 / hertz)) * (2.0 / PI) - (PI / 2.0);

        case 5: // pseudo random noise
            return 2.0 * ((double) rand() / (double) RAND_MAX) - 1.0;

        default:
            return 0.0;

    }


}

// ritorna ampiezza (tra -1 e 1) in funzione del tempo
double make_noise(double time) {

    double output = osc(frequency, time, 3);

    return output * 0.5; // lower volume

}

int main() {

    // get all sound hw
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

    // display
    for (auto d: devices) wcout << "Found Output Device: " << d << endl;

    // create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // link noise function with sound machine
    sound.SetUserFunction(make_noise);

    print_keyboard();

    while (1) {

        bool key_pressed = false;
        for (int i = 0; i < 13; ++i) {
            if ( GetAsyncKeyState( (unsigned char)("ZSXCFVGBNJMK\xbc")[i] ) && 0x8000 ) {
                
                frequency = octave_base_freq * pow(d12th_root_of_2, i);
                key_pressed = true;
            }
        }

        if (!key_pressed) {
            frequency = 0.0;
        }

    }

    return 0;
    
}