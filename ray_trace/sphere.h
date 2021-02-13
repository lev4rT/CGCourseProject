#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object
{
private:
    std::vector<double> center;
    double r;
    std::vector<int> color;
    double specular;
    double reflective;
public:
    Sphere(std::vector<double> ce, double radius, std::vector<int> co, double spec, double ref) :
        center(ce), r(radius), color(co), specular(spec), reflective(ref) {}

    virtual std::vector<double> IntersectRay(const std::vector<double>& O, const std::vector<double>& D) override;
    virtual std::vector<double>getCenter() override;
    virtual std::vector<int> getColor() override;
    virtual double getRadius() override;
    virtual double getSpecular() override;
    virtual double getReflective() override;
    virtual void setCenter(const std::vector<double>&) override;
};

#endif // SPHERE_H
