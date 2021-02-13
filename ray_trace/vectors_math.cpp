#include "vectors_math.h"

double VectorLength(const std::vector<double>& v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

std::vector<double> VectorSum(const std::vector<double>& u, const std::vector<double>& v) {
    return {v[0] + u[0], v[1] + u[1], v[2] + u[2]};
}

std::vector<int> VectorSum(const std::vector<int>& u, const std::vector<int>& v) {
    return {v[0] + u[0], v[1] + u[1], v[2] + u[2]};
}

std::vector<double> VectorMult(const std::vector<double>& u, const double& v) {
    return {u[0] * v, u[1] * v, u[2] * v};
}

std::vector<int> VectorMult(const std::vector<int>& u, const double& v) {
    return {(int) round(u[0] * v), (int) round(u[1] * v), (int) round(u[2] * v)};
}

std::vector<int> VectorMult(const std::vector<int>& u, const std::vector<double>& v) {
    return {(int) round(u[0] * v[0]), (int) round(u[1] * v[1]), (int) round(u[2] * v[2])};
}


std::vector<double> VectorDiff(const std::vector<double>& u, const std::vector<double>& v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

double VectorScalarMult(const std::vector<double>& U, const std::vector<double>& V) {
    return U[0] * V[0] + U[1] * V[1] + U[2] * V[2];
}

std::vector<double> ReflectRay(const std::vector<double>& R, const std::vector<double>& N) {
    return VectorDiff(VectorMult(N, 2 * VectorScalarMult(N, R)), R);
}

std::vector<std::vector<double>> XSpinMatrixCalculate(double alpha) {
    alpha *= M_PI / 180; // degrees to radians!
    return {{1, 0, 0},
            {0, cos(alpha), sin(alpha)},
            {0, -sin(alpha), cos(alpha)}};
}

std::vector<std::vector<double>> YSpinMatrixCalculate(double alpha) {
    alpha *= M_PI / 180; // degrees to radians!
    return {{cos(alpha), 0, -sin(alpha)},
            {0, 1, 0},
            {sin(alpha), 0, cos(alpha)}};
}

std::vector<std::vector<double>> ZSpinMatrixCalculate(double alpha) {
    alpha *= M_PI / 180; // degrees to radians!
    return {{cos(alpha), sin(alpha), 0},
            {-sin(alpha), cos(alpha), 0},
            {0, 0, 1}};
}

std::vector<double> MatrixOnVectorMult(const std::vector<std::vector<double>>& M, const std::vector<double>& v) {
    return {{M[0][0] * v[0] + M[0][1] * v[1] + M[0][2] * v[2]},
            {M[1][0] * v[0] + M[1][1] * v[1] + M[1][2] * v[2]},
            {M[2][0] * v[0] + M[2][1] * v[1] + M[2][2] * v[2]}};
}
