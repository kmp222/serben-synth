#pragma once

#include "preset.hpp"

#define M_PI 3.14159265358979323846

// return angular velocity given frequency
double w(double hertz);

// custom simple GUI
void print_keyboard();
void print_info();

// preset serialization
std::string preset_to_json(Preset p);

Preset json_to_preset(const std::string &s);