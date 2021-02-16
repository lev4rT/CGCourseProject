#include "object.h"

Object::Object(Object::type o_type,
               std::vector<int> color,
               double specular,
               double reflective,
               double transp) :
    object_type (o_type), color(color), specular(specular), reflective(reflective), transparency(transp) { }


Object::~Object() { }
