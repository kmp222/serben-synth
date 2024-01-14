#include <iostream>
#include <atomic>
#include "olcNoiseMaker.h"

// TODO
// 1) organizzare in file diversi
// 2) sistemare transient click
// 3) amplitude check x safety
// 4) cambio osc a runtime
// 6) sequencer
// 7) GUI
// 8) dist effect


struct envelopeADSR {

    double attack_time;
    double decay_time;
    double release_time;

    double start_amplitude;
    double sustain_amplitude;

    double trigger_on_time;
    double trigger_off_time;

    bool note_is_on;

    envelopeADSR() {
        
        attack_time = 0.10;
        decay_time = 0.01;
        release_time = 0.20;
        start_amplitude = 1.0;
        sustain_amplitude = 0.8;
        trigger_on_time = 0.0;
        trigger_off_time = 0.0;
        note_is_on = false;

    }

    double get_amplitude(double time) {

        double amplitude = 0.0;
        double lifetime = time - trigger_on_time;

        if (note_is_on) {
            
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

            // release
            amplitude = ((time - trigger_off_time) / release_time) *
                (0.0 - sustain_amplitude) + sustain_amplitude;

        }

        if (amplitude <= 0.0001) {
            amplitude = 0.0;
        }

        return amplitude;

    }

    void note_on(double time_on) {

        trigger_on_time = time_on;
        note_is_on = true;

    }

    void note_off(double time_off) {

        trigger_off_time = time_off;
        note_is_on = false;

    }

};

std::atomic<double> frequency(0.0);
double octave_base_freq = 110; // A2
double d12th_root_of_2 = pow(2.0, 1.0 / 12.0);
envelopeADSR envelope;

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
                
                for (double i = 1.0; i < 50.0; ++i) {
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

    double output = envelope.get_amplitude(time) * osc(frequency, time, 3);

    return output;

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

            if ( GetAsyncKeyState( (unsigned char)("ZSXCFVGBNJMK\xbc")[i] ) && 0x8000 ) {
                
                if (current_key != i) {
                    frequency = octave_base_freq * pow(d12th_root_of_2, i);
                    envelope.note_on(sound.GetTime());
                    current_key = i;
                }
                
                key_pressed = true;

            }

        }

        if (!key_pressed) {

            if (current_key != -1) {
                envelope.note_off(sound.GetTime());
                current_key = -1;
            }
            
        }

    }

    return 0;
    
}