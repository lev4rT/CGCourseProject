#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <iostream>
#include <QThread>
#include <QMutex>
#include <QColor>

#include "ray_tracing_functions.h"
#include "pixel.h"

//std::ref(xStart), std::ref(xEnd),
//                                                             std::ref(Vw), std::ref(Vh),
//                                                             std::ref(d),
//                                                             std::ref(Cw), std::ref(Ch),
//                                                             std::ref(MatrixSpin),
//                                                             std::ref(RECURSIVE_DEPTH),
//                                                             std::ref(O),
//                                                             std::ref(PixelMap),
//                                                             std::ref(objects),
//                                                             std::ref(lights),
//                                                             BG_COLOR,
//                                                             RECURSIVE_DEPTH);

void CalculateRays(const double xStart, const double xEnd,
                   const double& Vw,
                   const double& Vh,
                   const double& d,
                   const double& Cw,
                   const double& Ch,
                   const std::vector<std::vector<double>>& MatrixSpin,
                   const std::vector<double>& O,
                   std::vector<std::vector<Pixel>>& PixelMap,
                   const std::vector<Object*>& objects,
                   const std::vector<Light>& lights,
                   const std::vector<int> BG_COLOR,
                   const int RECURSIVE_DEPTH);

#endif // MYTHREAD_H
