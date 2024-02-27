#include <iostream>

#include "utils.hpp"
#include "nlohmann/json.hpp"

double w(double hertz) {
    return hertz * 2.0 * M_PI;
}

/* void print_keyboard() {

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
} */

using json = nlohmann::json;

std::string preset_to_json(Preset p) {

    json j;

    j["attack"] = p.attack;
    j["decay"] = p.decay;
    j["release"] = p.release;
    j["start"] = p.start;
    j["sustain"] = p.sustain;

    json oscillatorsJson = json::array();

    oscillatorsJson.push_back({
        {"wave", static_cast<int>(p.wave)},
        {"volume", p.o_volume},
        {"lfo_hertz", p.lfo_hertz},
        {"lfo_amplitude", p.lfo_amplitude},
        {"transpose", p.transpose}
    });
    oscillatorsJson.push_back({
        {"wave", static_cast<int>(p.wave2)},
        {"volume", p.o2_volume},
        {"lfo_hertz", p.lfo_hertz2},
        {"lfo_amplitude", p.lfo_amplitude2},
        {"transpose", p.transpose2}
    });
    oscillatorsJson.push_back({
        {"wave", static_cast<int>(p.wave3)},
        {"volume", p.o3_volume},
        {"lfo_hertz", p.lfo_hertz3},
        {"lfo_amplitude", p.lfo_amplitude3},
        {"transpose", p.transpose3}
    });

    j["oscillators"] = oscillatorsJson;

    return j.dump();
    
}

Preset json_to_preset(const std::string &jsonString) {

    json j = json::parse(jsonString);

    Preset preset;
    preset.attack = j["attack"];
    preset.decay = j["decay"];
    preset.release = j["release"];
    preset.start = j["start"];
    preset.sustain = j["sustain"];

    preset.wave = static_cast<Osc::Waves>(j["oscillators"][0]["wave"]);
    preset.o_volume = j["oscillators"][0]["volume"];
    preset.lfo_hertz = j["oscillators"][0]["lfo_hertz"];
    preset.lfo_amplitude = j["oscillators"][0]["lfo_amplitude"];
    preset.transpose = j["oscillators"][0]["transpose"];

    preset.wave2 = static_cast<Osc::Waves>(j["oscillators"][1]["wave"]);
    preset.o2_volume = j["oscillators"][1]["volume"];
    preset.lfo_hertz2 = j["oscillators"][1]["lfo_hertz"];
    preset.lfo_amplitude2 = j["oscillators"][1]["lfo_amplitude"];
    preset.transpose2 = j["oscillators"][1]["transpose"];

    preset.wave3 = static_cast<Osc::Waves>(j["oscillators"][2]["wave"]);
    preset.o3_volume = j["oscillators"][2]["volume"];
    preset.lfo_hertz3 = j["oscillators"][2]["lfo_hertz"];
    preset.lfo_amplitude3 = j["oscillators"][2]["lfo_amplitude"];
    preset.transpose3 = j["oscillators"][2]["transpose"];

    return preset;

}
