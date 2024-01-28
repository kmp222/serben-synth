#include <iostream>
#include <cmath>

#include "olcNoiseMaker.h"

#include "utils.hpp"
#include "synth.hpp"
#include "preset.hpp"

// sound tuning
const double BASE_FREQUENCY = 220.0;
const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0);

// synth
Synth s;

int main() {
    
    // get sound hw
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

    // create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // link synth sound to the sound machine
    sound.SetUserFunction([](double time) { return s.master_sound(time); });

    print_keyboard();

    print_info();

    // track key pressing
    bool key_pressed = false;
    int current_key = -1;

    // exit condition
    bool exit = false;

    // listen and wait for keyboard events
    while (!exit) {

        key_pressed = false;
        
        // check if any of the 12 notes is being pressed
        for (int i = 0; i < 13; ++i) {

            // if a note is being pressed, we generate the correct frequency sound
            if ( (GetAsyncKeyState( (unsigned char)("ZSXCFVGBNJMK\xbc")[i] ) & 0x8000) != 0 ) {
                
                // if a new note is captured, we set the correct frequency
                if (current_key != i) {
                    s.frequency = BASE_FREQUENCY * pow(SEMITONE_RATIO, i);
                    s.env.note_on(sound.GetTime());
                    current_key = i;
                }
                
                // if the note hasn't changed, we don't need to set the frequency again
                key_pressed = true;

            } 
        }

        // note released
        if (!key_pressed) {

            if (current_key != -1) {
                s.env.note_off(sound.GetTime());
                current_key = -1;
                s.frequency = 0.0;
            }
            
        }

        // pressing 0 will terminate the program
        if (GetAsyncKeyState(0x30)) {
            sound.Stop();
            exit = true;
        }

    }

    return 0;
    
}