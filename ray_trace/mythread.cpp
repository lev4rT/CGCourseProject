#include "mythread.h"

QMutex mutex;

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
                   const int RECURSIVE_DEPTH)
{
    std::vector<Pixel> PixelVector;
    std::cout << xStart << " " << xEnd << std::endl;
    for (int x = xStart; x < xEnd; x++) {
        for (int y = -Ch / 2; y < Ch / 2; y++) {
            std::vector<double> D = CanvasToViewport(x, y, Vw, Vh, Cw, Ch, d);
//            std::cout << D[0] << " " << D[1] << " " << D[2] << std::endl;
            if (!MatrixSpin.empty()) {D = MatrixOnVectorMult(MatrixSpin, D);}
            std::vector<int> color = TraceRay(O, D, 1, std::numeric_limits<double>::max(), RECURSIVE_DEPTH, BG_COLOR,
                                              objects, lights);
//            std::cout << color[0] << " " << color[1] << " " << color[2] << std::endl;
            PixelVector.push_back({(int) Cw / 2 + x, (int) Ch / 2 - y, color});
        }
    }
    mutex.lock();
    PixelMap.push_back(PixelVector);
    mutex.unlock();
}
