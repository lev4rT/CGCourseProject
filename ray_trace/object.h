#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <limits>

class Object
{
public:
    enum type {
        sphere = 0,
        plane,
        sphereSouceColor,
    };

public:
    type object_type;
    std::vector<int> color;
    double specular;
    double reflective;
    double transparency;

public:
    Object(type o_type, std::vector<int> color, double specular, double reflective, double transparency);
    virtual ~Object();

    virtual std::vector<double> IntersectRay(const std::vector<double>& O, const std::vector<double>& D) = 0;

    // Similar methods
    virtual std::vector<int> getColor() { return {}; }
    virtual double getSpecular() { return 0; }
    virtual double getReflective() { return 0; }
    double getTransparency() { return transparency; }

    // Sphere methods
    virtual std::vector<double>getCenter() { return {}; }
    virtual void setCenter(const std::vector<double>&) { }
    virtual double getRadius() { return 0; }

    // Plane methods
    virtual std::vector<double> getNormal() { return {}; }

    virtual double getMinX() { return {}; }
    virtual double getMaxX() { return {}; }
    virtual double getMinY() { return {}; }
    virtual double getMaxY() { return {}; }
    virtual double getMinZ() { return {}; }
    virtual double getMaxZ() { return {}; }

};

#endif // OBJECT_H
