#ifndef PLANE_H
#define PLANE_H

#include <iostream>

#include "object.h"
#include "vectors_math.h"

class Plane : public Object
{
public:
    // Ax + By + Cz + D = 0
    double A;
    double B;
    double C;
    double D;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;

public:
    Plane(double A, double B,
          double C, double D,
          std::vector<int> co, double spec, double ref, double transp,
          double x_min, double x_max,
          double y_min, double y_max,
          double z_min, double z_max);

    virtual std::vector<double> IntersectRay(const std::vector<double>& O, const std::vector<double>& D) override;
    virtual std::vector<int> getColor() override;

    virtual std::vector<double> getNormal() override;

    virtual double getMinX() override;
    virtual double getMaxX() override;
    virtual double getMinY() override;
    virtual double getMaxY() override;
    virtual double getMinZ() override;
    virtual double getMaxZ() override;
};

#endif // PLANE_H
