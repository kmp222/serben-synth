#include <iostream>
#include "olcNoiseMaker.h"
#include "synth.hpp"

Synth s;

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

// ritorna ampiezza (tra -1 e 1) in funzione del tempo
    static double make_noise(double time) {

        double output = s.voice->sound(time, s.frequency);
        return output * s.master_volume;

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
                    s.frequency = s.octave_base_freq * pow(s.twelfth_root_of_two, i);
                    s.voice->env.note_on(sound.GetTime());
                    current_key = i;
                }
                
                key_pressed = true;

            }

        }

        if (!key_pressed) {

            if (current_key != -1) {
                s.voice->env.note_off(sound.GetTime());
                current_key = -1;
                s.frequency = 0.0;
            }
            
        }

    }
    
    delete s.voice;
    s.voice = nullptr;

    return 0;
    
}