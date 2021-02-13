#ifndef RAY_TRACING_FUNCTIONS_H
#define RAY_TRACING_FUNCTIONS_H

#include <vector>

#include "vectors_math.h"
#include "object.h"
#include "light.h"

std::vector<double> CanvasToViewport(double x, double y,
                                     const double& Vw, const double& Vh,
                                     const double& Cw, const double& Ch,
                                     const double& d);

void ClosestIntersection (const std::vector<double>& O,
                          const std::vector<double>& D,
                          const double& t_min,
                          const double& t_max,
                          double& closest_t,
                          Object*& closest_object,
                          const std::vector<Object*>& objects);

std::vector<double> ComputeLightning(const std::vector<double>& P,
                                     const std::vector<double>& N,
                                     const std::vector<double>& V,
                                     const double& s,
                                     const std::vector<Light>& lights,
                                     const std::vector<Object*>& objects);

std::vector<int> TraceRay(const std::vector<double>& O,
                          const std::vector<double>& D,
                          double t_min,
                          double t_max,
                          int depth,
                          const std::vector<int>& BG_COLOR,
                          const std::vector<Object*>& objects,
                          const std::vector<Light>& lights);

#endif // RAY_TRACING_FUNCTIONS_H
