#include "plane.h"

Plane::Plane(double A, double B,
             double C, double D,
             std::vector<int> co, double spec, double ref, double transp,
             double x_mn, double x_mx,
             double y_mn, double y_mx,
             double z_mn, double z_mx) :
    Object(Object::type::plane, co, spec, ref, transp),
    A(A), B(B), C(C), D(D),
    x_min(x_mn), x_max(x_mx),
    y_min(y_mn), y_max(y_mx),
    z_min(z_mn), z_max(z_mx) { }


std::vector<double> Plane::IntersectRay(const std::vector<double>& O, const std::vector<double>& D)  {
    std::vector<double> V = {A, B, C};
    std::vector<double> C = {0, 0, 0};  // Point that lies on Plane
//    if (fabs(A) > 1e-6) {
//        C = {0, 0, -this->D / this->A};
//    } else if (fabs(B) > 1e-6) {
//        C = {0, 0, -this->D / this->B};
//    } else {
//        C = {0, 0, -this->D / this->C};
//    }
    // Normalize
    V = {V[0] / VectorLength(V),
         V[1] / VectorLength(V),
         V[2] / VectorLength(V)};

    std::vector<double> OC = VectorDiff(O, C);

    double DV = VectorScalarMult(D, V);
    double OCV = VectorScalarMult(OC, V);

    if (fabs(DV) > 1e-6) {
        double t = - OCV / DV;
        if (t < 0) {
            return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
        }
        std::cout << t << std::endl;
        return {t, t};
    }
    return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
}

std::vector<int> Plane::getColor() {
    return color;
}

std::vector<double> Plane::getNormal() {
    std::vector<double> V = {A, B, C};
    return {V[0] / VectorLength(V),
            V[1] / VectorLength(V),
            V[2] / VectorLength(V)};
}


double Plane::getMinX() { return x_min; }
double Plane::getMaxX() { return x_max; }
double Plane::getMinY() { return y_min; }
double Plane::getMaxY() { return y_max; }
double Plane::getMinZ() { return z_min; }
double Plane::getMaxZ() { return z_max; }
