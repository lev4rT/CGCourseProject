#include "vectors_math.h"

double VectorLength(const std::vector<double>& v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

std::vector<double> VectorSum(const std::vector<double>& u, const std::vector<double>& v) {
    return {v[0] + u[0], v[1] + u[1], v[2] + u[2]};
}

std::vector<double> VectorMult(const std::vector<double>& u, const double& v) {
    return {u[0] * v, u[1] * v, u[2] * v};
}

std::vector<int> VectorMult(const std::vector<int>& u, const double& v) {
    return {(int) round(u[0] * v), (int) round(u[1] * v), (int) round(u[2] * v)};
}


std::vector<double> VectorDiff(const std::vector<double>& u, const std::vector<double>& v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

double VectorScalarMult(const std::vector<double>& U, const std::vector<double>& V) {
    return U[0] * V[0] + U[1] * V[1] + U[2] * V[2];
}
