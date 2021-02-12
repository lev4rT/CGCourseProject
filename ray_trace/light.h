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
    Light(type t, double intensity, std::vector<double> position = {}, std::vector<double> direction = {});
    type t;
    double intensity;
    std::vector<double> position;
    std::vector<double> direction;

    void setPosition(std::vector<double> newPosition);
};

#endif // LIGHT_H
