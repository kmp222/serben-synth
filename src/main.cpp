#include <iostream>
#include <cmath>
#include <map>

#include "olcNoiseMaker.h"

#include "utils.hpp"
#include "synth.hpp"

#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Simple_Counter.H>

double BASE_FREQUENCY = 220.0; // sound tuning
const double SEMITONE_RATIO = pow(2.0, 1.0 / 12.0); // sound tuning
Synth s; // singleton
vector<wstring> devices = olcNoiseMaker<short>::Enumerate(); // sound hw
olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512); // sound machine
boolean is_key_pressed = false;
char key_pressed = ' ';

std::map<int, int> keyboard = {
    {0x7A, 0},  // z
    {0x73, 1},  // s
    {0x78, 2},  // x
    {0x63, 3},  // c
    {0x66, 4},  // f
    {0x76, 5},  // v
    {0x67, 6},  // g
    {0x62, 7},  // b
    {0x6E, 8},  // n
    {0x6A, 9},  // j
    {0x6D, 10}, // m
    {0x6B, 11}, // k
    {0x2C, 12}  // ,
};

void play_sound(int key) {
    
    // if a new note is captured, we set the correct frequency
    if (keyboard.find(key) != keyboard.end()) {

        s.frequency = BASE_FREQUENCY * pow(SEMITONE_RATIO, keyboard.at(key));
        s.env.note_on(sound.GetTime());
        key_pressed = key;

    }
   
    // if the note hasn't changed, we don't need to set the frequency again
    is_key_pressed = true;
   
}

void release_sound() {

    if (!is_key_pressed) {

        s.env.note_off(sound.GetTime());
        key_pressed = ' ';
        s.frequency = 0.0;

    }

}

// override handle method
class MyWindow : public Fl_Window {

public:
    MyWindow(int w, int h, const char* title) : Fl_Window(w, h, title) {}

    int handle(int event) override {

        is_key_pressed = false;

        switch (event) {
        case FL_KEYDOWN:
            play_sound(Fl::event_key());
            return 1;
        case FL_KEYUP:
            release_sound();
            return 1;
        case FL_HIDE:
            close();
        default:
            return Fl_Window::handle(event);
        }

    }

    void close() {
        sound.Stop(); 
        Fl_Window::hide();
    }

};

// generic slider callback
template<typename T>
void slider_callback(Fl_Widget* widget, void* data) {

    Fl_Value_Slider* slider = static_cast<Fl_Value_Slider*>(widget);
    T* value_ptr = static_cast<T*>(data);
    *value_ptr = static_cast<T>(slider->value());

}

// waves choice callback
void choice_callback(Fl_Widget* widget, void* data) {

    Fl_Choice* choice = static_cast<Fl_Choice*>(widget);
    Osc::Waves* wave = static_cast<Osc::Waves*>(data);
    *wave = static_cast<Osc::Waves>(choice->value());

}

// transpose choice callback
void counter_callback(Fl_Widget* widget, void* data) {

    Fl_Counter* counter = static_cast<Fl_Counter*>(widget);
    double* transpose = static_cast<double*>(data);
    *transpose = static_cast<double>(counter->value());

}


// CLI main
/* int main() {

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
    
} */

// wop gui main
int main() {

    // link synth sound to the sound machine
    sound.SetUserFunction([](double time) { return s.master_sound(time); });

    // gui setup
    MyWindow window(800, 600, "Serben Synth");
    Fl_Box b(700, 500, 0, 0);
    Fl_Image* image = new Fl_PNG_Image("src/resources/images/serben.png");
    Fl_Image* scaled_image = image->copy(100, 100);
    delete image;
    image = nullptr;
    b.image(scaled_image);

    // attack slider
    double* attack = &(s.env.attack_time);
    Fl_Value_Slider* attack_slider = new Fl_Value_Slider(10, 10, 280, 25, "attack"); // x, y, width, length, name
    attack_slider->type(FL_HORIZONTAL);
    attack_slider->bounds(0, 10); // bounds
    attack_slider->step(0.01); // increment
    attack_slider->value(*attack); // init value
    attack_slider->callback(slider_callback<double>, attack);

    // decay slider
    double* decay = &(s.env.decay_time);
    Fl_Value_Slider* decay_slider = new Fl_Value_Slider(10, 60, 280, 25, "decay");
    decay_slider->type(FL_HORIZONTAL);
    decay_slider->bounds(0, 10);
    decay_slider->step(0.01);
    decay_slider->value(*decay);
    decay_slider->callback(slider_callback<double>, decay);

    // sustain slider
    double* sustain = &(s.env.sustain_amplitude);
    Fl_Value_Slider* sustain_slider = new Fl_Value_Slider(10, 110, 280, 25, "sustain volume");
    sustain_slider->type(FL_HORIZONTAL);
    sustain_slider->bounds(0, 1);
    sustain_slider->step(0.01);
    sustain_slider->value(*sustain);
    sustain_slider->callback(slider_callback<double>, sustain);

    // release slider
    double* release = &(s.env.release_time);
    Fl_Value_Slider* release_slider = new Fl_Value_Slider(10, 160, 280, 25, "release");
    release_slider->type(FL_HORIZONTAL);
    release_slider->bounds(0, 10);
    release_slider->step(0.01);
    release_slider->value(*release);
    release_slider->callback(slider_callback<double>, release);

    // volume post attack
    double* start_volume = &(s.env.start_amplitude);
    Fl_Value_Slider* start_volume_slider = new Fl_Value_Slider(10, 210, 280, 25, "start volume");
    start_volume_slider->type(FL_HORIZONTAL);
    start_volume_slider->bounds(0, 1);
    start_volume_slider->step(0.01);
    start_volume_slider->value(*start_volume);
    start_volume_slider->callback(slider_callback<double>, start_volume);

    // master volume
    double* master_volume = &(s.master_volume);
    Fl_Value_Slider* master_volume_slider = new Fl_Value_Slider(510, 10, 280, 25, "master volume");
    master_volume_slider->type(FL_HORIZONTAL);
    master_volume_slider->bounds(0, 1);
    master_volume_slider->step(0.01);
    master_volume_slider->value(*master_volume);
    master_volume_slider->callback(slider_callback<double>, master_volume);

    // three lfo's are unnecessary so only one is exposed to the user
    // lfo hertz
    double* lfo_hertz = &(s.osc1.lfo_hertz);
    Fl_Value_Slider* lfo_hertz_slider = new Fl_Value_Slider(510, 60, 100, 25, "lfo hertz");
    lfo_hertz_slider->type(FL_HORIZONTAL);
    lfo_hertz_slider->bounds(0, 20);
    lfo_hertz_slider->step(0.01);
    lfo_hertz_slider->value(*lfo_hertz);
    lfo_hertz_slider->callback(slider_callback<double>, lfo_hertz);

    // lfo amplitude
    double* lfo_amp = &(s.osc1.lfo_amplitude);
    Fl_Value_Slider* lfo_amp_slider = new Fl_Value_Slider(510, 110, 100, 25, "lfo amplitude");
    lfo_amp_slider->type(FL_HORIZONTAL);
    lfo_amp_slider->bounds(0, 1);
    lfo_amp_slider->step(0.01);
    lfo_amp_slider->value(*lfo_amp);
    lfo_amp_slider->callback(slider_callback<double>, lfo_amp);

    // osc1 transpose
    double* osc1_transpose = &(s.osc1.transpose);
    Fl_Simple_Counter* osc1_transpose_counter = new Fl_Simple_Counter(600, 260, 50, 25, "osc1 transpose");
    osc1_transpose_counter->type(FL_HORIZONTAL);
    osc1_transpose_counter->bounds(-24, 24);
    osc1_transpose_counter->step(1);
    osc1_transpose_counter->value(*osc1_transpose);
    osc1_transpose_counter->callback(counter_callback, osc1_transpose);

    // osc2 transpose
    double* osc2_transpose = &(s.osc2.transpose);
    Fl_Simple_Counter* osc2_transpose_counter = new Fl_Simple_Counter(600, 310, 50, 25, "osc2 transpose");
    osc2_transpose_counter->type(FL_HORIZONTAL);
    osc2_transpose_counter->bounds(-24, 24);
    osc2_transpose_counter->step(1);
    osc2_transpose_counter->value(*osc2_transpose);
    osc2_transpose_counter->callback(counter_callback, osc2_transpose);

    // osc3 transpose
    double* osc3_transpose = &(s.osc3.transpose);
    Fl_Simple_Counter* osc3_transpose_counter = new Fl_Simple_Counter(600, 360, 50, 25, "osc3 transpose");
    osc3_transpose_counter->type(FL_HORIZONTAL);
    osc3_transpose_counter->bounds(-24, 24);
    osc3_transpose_counter->step(1);
    osc3_transpose_counter->value(*osc3_transpose);
    osc3_transpose_counter->callback(counter_callback, osc3_transpose);

    // osc1 volume
    double* osc1_volume = &(s.osc1.volume);
    Fl_Value_Slider* osc1_volume_slider = new Fl_Value_Slider(400, 260, 100, 25, "osc1 volume");
    osc1_volume_slider->type(FL_HORIZONTAL);
    osc1_volume_slider->bounds(0, 1);
    osc1_volume_slider->step(0.01);
    osc1_volume_slider->value(*osc1_volume);
    osc1_volume_slider->callback(slider_callback<double>, osc1_volume);

    // osc2 volume
    double* osc2_volume = &(s.osc2.volume);
    Fl_Value_Slider* osc2_volume_slider = new Fl_Value_Slider(400, 310, 100, 25, "osc2 volume");
    osc2_volume_slider->type(FL_HORIZONTAL);
    osc2_volume_slider->bounds(0, 1);
    osc2_volume_slider->step(0.01);
    osc2_volume_slider->value(*osc2_volume);
    osc2_volume_slider->callback(slider_callback<double>, osc2_volume);

    // osc3 volume
    double* osc3_volume = &(s.osc3.volume);
    Fl_Value_Slider* osc3_volume_slider = new Fl_Value_Slider(400, 360, 100, 25, "osc3 volume");
    osc3_volume_slider->type(FL_HORIZONTAL);
    osc3_volume_slider->bounds(0, 1);
    osc3_volume_slider->step(0.01);
    osc3_volume_slider->value(*osc3_volume);
    osc3_volume_slider->callback(slider_callback<double>, osc3_volume);
    
    // osc1 wave
    Fl_Choice* osc1_wave = new Fl_Choice(60, 260, 280, 25, "osc 1");
    Osc::Waves* wave = &(s.osc1.wave);
    osc1_wave->add("sine");
    osc1_wave->add("square");
    osc1_wave->add("triangle");
    // osc1_wave->add("analog saw");
    osc1_wave->add("saw");
    osc1_wave->add("noise");
    osc1_wave->value(*wave);
    osc1_wave->callback(choice_callback, wave);

    // osc2 wave
    Fl_Choice* osc2_wave = new Fl_Choice(60, 310, 280, 25, "osc 2");
    Osc::Waves* wave2 = &(s.osc2.wave);
    osc2_wave->add("sine");
    osc2_wave->add("square");
    osc2_wave->add("triangle");
    // osc2_wave->add("analog saw");
    osc2_wave->add("saw");
    osc2_wave->add("noise");
    osc2_wave->value(*wave2);
    osc2_wave->callback(choice_callback, wave2);

    // osc3 wave
    Fl_Choice* osc3_wave = new Fl_Choice(60, 360, 280, 25, "osc 3");
    Osc::Waves* wave3 = &(s.osc3.wave);
    osc3_wave->add("sine");
    osc3_wave->add("square");
    osc3_wave->add("triangle");
    // osc3_wave->add("analog saw");
    osc3_wave->add("saw");
    osc3_wave->add("noise");
    osc3_wave->value(*wave3);
    osc3_wave->callback(choice_callback, wave3);

    // handling and display
    window.handle(FL_KEYDOWN | FL_KEYUP);
    window.end();
    window.show();

    return Fl::run();

}