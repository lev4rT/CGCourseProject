#include "pixel.h"

Pixel::Pixel(int x, int y, std::vector<int>& color_v) :
x(x), y(y), color(color_v[0], color_v[1], color_v[2]) { }
