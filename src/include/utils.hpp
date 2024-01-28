#pragma once

#include "preset.hpp"

#define M_PI 3.14159265358979323846

// return angular velocity given frequency
double w(double hertz);

// custom simple GUI
void print_keyboard();
void print_info();

void file_to_preset(Preset p);