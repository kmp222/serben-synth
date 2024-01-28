#include <iostream>

#include "utils.hpp"

double w(double hertz) {
    return hertz * 2.0 * M_PI;
}

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

void print_info() {
    std::cout << "press 0 to terminate the program" << std::endl;
}
