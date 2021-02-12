#include "light.h"

Light::Light(type tp, double intens, std::vector<double> pos, std::vector<double> dir) :
    t (tp), intensity(intens), position(pos), direction(dir) { }


void Light::setPosition(std::vector<double> newPosition) {
    position = newPosition;
}
