#include <iostream>
#include <cmath>
#include "olcNoiseMaker.h"
#include "synth.hpp"
#include "utils.hpp"
#include "preset.hpp"

const double BASE_FREQUENCY = 220.0;
const double TRANSPOSE = pow(2.0, 1.0 / 12.0);

Preset myPreset("sine", 0.0, 0.0, 0.0, 1.0, 1.0,
                   Waves::SINE, 1.0, 0.0, 0.0,
                   Waves::SINE, 0.0, 0.0, 0.0,
                   Waves::SINE, 0.0, 0.0, 0.0);

Synth s(myPreset);

// ritorna ampiezza (tra -1 e 1) in funzione del tempo
    static double make_noise(double time) {

        return s.master_sound(time);

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

    bool key_pressed = false;
    int current_key = -1;

    while (true) {

        key_pressed = false;

        for (int i = 0; i < 13; ++i) {

            if ( (GetAsyncKeyState( (unsigned char)("ZSXCFVGBNJMK\xbc")[i] ) & 0x8000) != 0 ) {
                
                if (current_key != i) {
                    s.frequency = BASE_FREQUENCY * pow(TRANSPOSE, i);
                    s.env.note_on(sound.GetTime());
                    current_key = i;
                }
                
                key_pressed = true;

            }

        }

        if (!key_pressed) {

            if (current_key != -1) {
                s.env.note_off(sound.GetTime());
                current_key = -1;
                s.frequency = 0.0;
            }
            
        }

    }

    return 0;
    
}