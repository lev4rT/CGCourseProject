#ifndef VECTORS_MATH_H
#define VECTORS_MATH_H

#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>

double VectorLength(const std::vector<double>& v);

std::vector<double> VectorSum(const std::vector<double>& u, const std::vector<double>& v);
std::vector<int> VectorSum(const std::vector<int>& u, const std::vector<int>& v);
std::vector<double> VectorMult(const std::vector<double>& u, const double& v);
std::vector<int> VectorMult(const std::vector<int>& u, const double& v);
std::vector<int> VectorMult(const std::vector<int>& u, const std::vector<double>& v);
std::vector<double> VectorMult(const std::vector<double>& u, const std::vector<double>& v);
std::vector<double> MatrixOnVectorMult(const std::vector<std::vector<double>>& M, const std::vector<double>& v);
std::vector<double> VectorDiff(const std::vector<double>& u, const std::vector<double>& v);
double VectorScalarMult(const std::vector<double>& U, const std::vector<double>& V);
std::vector<double> ReflectRay(const std::vector<double>& R, const std::vector<double>& N);

std::vector<std::vector<double>> XSpinMatrixCalculate(double alpha);
std::vector<std::vector<double>> YSpinMatrixCalculate(double alpha);
std::vector<std::vector<double>> ZSpinMatrixCalculate(double alpha);

#endif // VECTORS_MATH_H
