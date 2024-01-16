#pragma once

#define M_PI 3.14159265358979323846

// list of all possible waves
enum Waves {
        
    SINE = 0,
    SQUARE = 1,
    TRIANGLE = 2,
    ANALOG_SAW = 3,
    DIGITAL_SAW = 4,
    NOISE = 5

};

// return angular velocity given frequency
double w(double hertz);

// custom simple GUI
void print_keyboard();
void print_info();