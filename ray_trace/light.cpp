#include "light.h"

Light::Light(type tp, std::vector<double> intens, std::vector<double> pos, std::vector<double> dir, double l_power) :
    t (tp), color_intensity(intens), position(pos), direction(dir), lightning_power(l_power) { }


void Light::setPosition(std::vector<double> newPosition) {
    position = newPosition;
}
