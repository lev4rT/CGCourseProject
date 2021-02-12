#ifndef VECTORS_MATH_H
#define VECTORS_MATH_H

#include <vector>
#include <cmath>

double VectorLength(const std::vector<double>& v);

std::vector<double> VectorSum(const std::vector<double>& u, const std::vector<double>& v);
std::vector<double> VectorMult(const std::vector<double>& u, const double& v);
std::vector<int> VectorMult(const std::vector<int>& u, const double& v);
std::vector<double> VectorDiff(const std::vector<double>& u, const std::vector<double>& v);
double VectorScalarMult(const std::vector<double>& U, const std::vector<double>& V);

#endif // VECTORS_MATH_H
