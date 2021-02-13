#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

class Light {
public:
    enum type {
        ambient = 0,
        point,
        directional
    };

public:
    type t;
    std::vector<double> color_intensity;
    std::vector<double> position;
    std::vector<double> direction;
    double lightning_power;

public:
    Light(type t, std::vector<double> intensity, std::vector<double> position = {}, std::vector<double> direction = {}, double l_power = 1);
    void setPosition(std::vector<double> newPosition);
};

#endif // LIGHT_H
