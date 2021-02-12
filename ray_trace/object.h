#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <limits>

class Object
{
public:
    Object();

    virtual ~Object();

    virtual std::vector<double> IntersectRay(const std::vector<double>& O, const std::vector<double>& D) = 0;
    virtual std::vector<double>getCenter() = 0;
    virtual std::vector<int> getColor() = 0;
    virtual void setCenter(const std::vector<double>&) = 0;

    // NOT ALL OBJECT WILL HAVE RADIUS. WE SHOULD THINK HOW TO SOLVE IT IN FUTURE.
    virtual double getRadius() = 0;
    virtual double getSpecular() = 0;

};

#endif // OBJECT_H
