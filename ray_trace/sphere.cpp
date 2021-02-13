#include "sphere.h"
#include "vectors_math.h"

std::vector<double> Sphere::IntersectRay(const std::vector<double>& O, const std::vector<double>& D) {
    std::vector<double> OC = {O[0] - center[0],
                              O[1] - center[1],
                              O[2] - center[2]};

    double k1 = VectorScalarMult(D, D);
    double k2 = 2 * VectorScalarMult(OC, D);
    double k3 = VectorScalarMult(OC, OC) - r * r;

    double discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0) {
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    }

    double t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
    double t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
    return {t1, t2};
}


std::vector<double> Sphere::getCenter() {
    return center;
}

std::vector<int> Sphere::getColor() {
    return color;
}

double Sphere::getRadius() {
    return r;
}

void Sphere::setCenter(const std::vector<double>& newCenter) {
    center = newCenter;
}

double Sphere::getSpecular() {
    return specular;
}

double Sphere::getReflective() {
    return reflective;
}
