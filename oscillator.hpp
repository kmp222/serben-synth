#pragma once

const int SINE = 0;
const int SQUARE = 1;
const int TRIANGLE = 2;
const int ANALOG_SAW = 3;
const int DIGITAL_SAW = 4;
const int NOISE = 5;

double w(double hertz);

double osc(double hertz, double time, int type, double lfo_hertz = 0.0, double lfo_amplitude = 0.0);