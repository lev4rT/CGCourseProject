#include "ray_tracing_functions.h"

std::vector<double> CanvasToViewport(double x, double y,
                                     const double& Vw, const double& Vh,
                                     const double& Cw, const double& Ch,
                                     const double& d) {
    double l1 = x * Vw / Cw;
    double l2 = y * Vh / Ch;
    return {x * Vw / Cw, y * Vh / Ch, d};
}

void ClosestIntersection (const std::vector<double>& O,
                          const std::vector<double>& D,
                          const double& t_min,
                          const double& t_max,
                          double& closest_t,
                          Object*& closest_object,
                          const std::vector<Object*>& objects) {
    for (size_t i = 0; i < objects.size(); i++) {
        std::vector<double> intersections = objects[i]->IntersectRay(O, D);
        if (intersections[0] > t_min && intersections[0] < t_max && intersections[0] < closest_t) {
            closest_t = intersections[0];
            closest_object= objects[i];
        }
        if (intersections[1] > t_min && intersections[1] < t_max && intersections[1] < closest_t) {
            closest_t = intersections[1];
            closest_object = objects[i];
        }
    }
}

std::vector<double> ComputeLightning(const std::vector<double>& P,
                        const std::vector<double>& N,
                        const std::vector<double>& V,
                        const double& s,
                        const std::vector<Light>& lights,
                        const std::vector<Object*>& objects) {
    std::vector<double> i = {0, 0, 0};
    std::vector<double> L;
    for (const auto& light : lights) {
        if (light.t == Light::type::ambient) {
            i = VectorSum(i, light.color_intensity);
        } else {
            double t_max;
            if (light.t == Light::type::point) {
                L = {light.position[0] - P[0],
                     light.position[1] - P[1],
                     light.position[2] - P[2]};
                t_max = 1;
            } else {
                L =light.direction;
                t_max = std::numeric_limits<double>::max();
            }

            // Shadow check
            double shadow_t = std::numeric_limits<double>::max();
            Object* closest_shadow_object = nullptr;
            ClosestIntersection(P, L, 1E-3, t_max, shadow_t, closest_shadow_object, objects);
            if (closest_shadow_object != nullptr) {
                continue;
            }

            // Diffusual
            double n_dot_l = VectorScalarMult(N, L);
            if (n_dot_l > 0) {
                if (light.color_intensity[0] > 0)
                    i[0] += light.color_intensity[0] * n_dot_l / (VectorLength(N) * VectorLength(L));
                else
                    i[0] -= light.color_intensity[0] * n_dot_l / (VectorLength(N) * VectorLength(L));

                if (light.color_intensity[1] > 0)
                    i[1] += light.color_intensity[1] * n_dot_l / (VectorLength(N) * VectorLength(L));
                else
                    i[1] -= light.color_intensity[1] * n_dot_l / (VectorLength(N) * VectorLength(L));

                if (light.color_intensity[2] > 0)
                    i[2] += light.color_intensity[2] * n_dot_l / (VectorLength(N) * VectorLength(L));
                else
                    i[2] -= light.color_intensity[2] * n_dot_l / (VectorLength(N) * VectorLength(L));
            }

            // Mirroring
            if (fabs(s + 1) > 1E-6) {
                std::vector<double> R = VectorDiff(VectorMult(VectorMult(N, 2), VectorScalarMult(N, L)), L);
                double r_dot_v = VectorScalarMult(R, V);
                if (r_dot_v > 0) {
                    i[0] += light.color_intensity[0] * pow(r_dot_v / (VectorLength(R) * VectorLength(V)), s);
                    i[1] += light.color_intensity[1] * pow(r_dot_v / (VectorLength(R) * VectorLength(V)), s);
                    i[2] += light.color_intensity[2] * pow(r_dot_v / (VectorLength(R) * VectorLength(V)), s);
                }
            }
        }
    }
    if (i[0] > 255) { i[0] = 255; }
    if (i[1] > 255) { i[1] = 255; }
    if (i[2] > 255) { i[2] = 255; }

    if (i[0] < 0) { i[0] = 0; }
    if (i[1] < 0) { i[1] = 0; }
    if (i[2] < 0) { i[2] = 0; }
    return i;
}

std::vector<int> TraceRay(const std::vector<double>& O,
                          const std::vector<double>& D,
                          double t_min,
                          double t_max,
                          int depth,
                          const std::vector<int>& BG_COLOR,
                          const std::vector<Object*>& objects,
                          const std::vector<Light>& lights) {
    double closest_t = std::numeric_limits<double>::max();
    Object* closest_object = nullptr;

    ClosestIntersection(O, D, t_min, t_max, closest_t, closest_object, objects);

    if (closest_object == nullptr) {
        return BG_COLOR;
    }

    std::vector<double> P = VectorSum(O, VectorMult(D, closest_t)); // Intersection point
    std::vector<double> N;  // Normal of the object in the P point
    if (closest_object->object_type == Object::type::sphere) {
        N = VectorDiff(P, closest_object->getCenter());
        N = {N[0] / VectorLength(N),
             N[1] / VectorLength(N),
             N[2] / VectorLength(N)};
    } else if (closest_object->object_type == Object::type::plane) {
//        if (P[0] >= closest_object->getMinX() && P[0] <= closest_object->getMaxX() &&
//            P[1] >= closest_object->getMinY() && P[1] <= closest_object->getMaxY() &&
//            P[2] >= closest_object->getMinZ() && P[2] <= closest_object->getMaxZ()) {
        N = closest_object->getNormal();
        if (VectorScalarMult(D, N) < 0) {
            N = VectorMult(N, -1);
        }
//        } else {
//            return BG_COLOR;
//        }

    }
    std::vector<int> local_color = VectorMult(closest_object->getColor(),
                                              ComputeLightning(P,
                                                               N,
                                                               VectorMult(D, -1),
                                                               closest_object->getSpecular(),
                                                               lights, objects));

    if (local_color[0] > 255) { local_color[0] = 255; }
    if (local_color[1] > 255) { local_color[1] = 255; }
    if (local_color[2] > 255) { local_color[2] = 255; }

    if (local_color[0] < 0) { local_color[0] = 0; }
    if (local_color[1] < 0) { local_color[1] = 0; }
    if (local_color[2] < 0) { local_color[2] = 0; }

    double r = closest_object->getReflective();
    if (depth <= 0 || r <= 0) {
        return local_color;
    }

    std::vector<double> R = ReflectRay(VectorMult(D, -1), N);
    std::vector<int> reflected_color = TraceRay(P, R, 1E-3, std::numeric_limits<double>::max(),
                                                depth - 1, BG_COLOR,
                                                objects, lights);

    return VectorSum(VectorMult(local_color, 1 - r), VectorMult(reflected_color, r));
}
