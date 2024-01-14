#include <iostream>
#include <atomic>
#include "olcNoiseMaker.h"

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
        
        attack_time = 0.01;
        decay_time = 1.0;
        release_time = 1.0;
        start_amplitude = 1.0;
        sustain_amplitude = 0.0;
        trigger_on_time = 0.0;
        trigger_off_time = 0.0;
        note_is_on = false;

    }

    double get_amplitude(double time) {

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

        if (amplitude <= 0.0) {
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

const double master_volume = 1.0;
std::atomic<double> frequency(0.0);
double octave_base_freq = 220; // A3
double twelfth_root_of_two = pow(2.0, 1.0 / 12.0);

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

double osc(double hertz, double time, int type, double lfo_hertz = 0.0, double lfo_amplitude = 0.0) {

    double freq = w(hertz) * time + lfo_amplitude * hertz * sin(w(lfo_hertz) * time);

    switch(type) {

        case 0: // sine
            return sin(freq);

        case 1: // square
            return sin(freq) > 0.0 ? 1.0 : -1.0;
        
        case 2: // triangle
            return asin( sin(freq) ) * (2.0 / PI);
        
        case 3: // saw (analog, warm)
            {
                double output = 0.0;
                
                for (double i = 1.0; i < 50.0; ++i) {
                    output += ( sin(i * freq)) / i;
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

struct instrument {

    double volume;
    envelopeADSR env;
    virtual ~instrument() {};

    virtual double sound(double time, double frequency) = 0;

};

struct simpleSine : public instrument {

    simpleSine() {
        volume = 0.5;
        env.attack_time = 0.0;
        env.decay_time = 0.0;
        env.start_amplitude = 1.0;
        env.sustain_amplitude = 1.0;
        env.release_time = 0.0;
    }

    double sound(double time, double frequency) {

        double s = env.get_amplitude(time) * 
            (

            + 1.0 * osc (frequency, time, 0)
    
            );

        return s * volume;

    }

};

struct bell : public instrument {

    bell() {
        volume = 0.25;
        env.attack_time = 0.01;
        env.decay_time = 1.0;
        env.start_amplitude = 1.0;
        env.sustain_amplitude = 0.0;
        env.release_time = 1.0;
    }

    double sound(double time, double frequency) {

        double s = env.get_amplitude(time) * 
            (

            + 1.0 * osc(frequency * 2.0, time, 0, 5.0, 0.001)
            + 0.5 * osc(frequency * 3.0, time, 0)
            + 0.25 * osc(frequency * 4.0, time, 0)
    
            );

        return s * volume;

    }

};

struct harmonica : public instrument {

    harmonica() {
        volume = 0.1;
        env.attack_time = 0.05;
        env.decay_time = 1.0;
        env.start_amplitude = 1.0;
        env.sustain_amplitude = 0.95;
        env.release_time = 2;
    }

    double sound(double time, double frequency) {

        double s = env.get_amplitude(time) * 
            (

            + 1.0 * osc(frequency, time, 1, 5.0, 0.001)
            + 0.5 * osc(frequency * 2.0, time, 1)
            + 0.05 * osc(frequency * 3.0, time, 5)
    
            );

        return s * volume;

    }

};

instrument *voice = nullptr;

// ritorna ampiezza (tra -1 e 1) in funzione del tempo
double make_noise(double time) {

    double output = voice->sound(time, frequency);
    return output * master_volume;

}

int main() {

    // get all sound hw
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

    // display
    for (auto d: devices) wcout << "Found Output Device: " << d << endl;

    // create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    voice = new bell();

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
                    frequency = octave_base_freq * pow(twelfth_root_of_two, i);
                    voice->env.note_on(sound.GetTime());
                    current_key = i;
                }
                
                key_pressed = true;

            }

        }

        if (!key_pressed) {

            if (current_key != -1) {
                voice->env.note_off(sound.GetTime());
                current_key = -1;
                frequency = 0.0;
            }
            
        }

    }

    
    delete voice;
    voice = nullptr;

    return 0;
    
}