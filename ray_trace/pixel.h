#ifndef PIXEL_H
#define PIXEL_H

#include <vector>
#include <QColor>

struct Pixel {
    int x;
    int y;
    QColor color;

    Pixel(int x, int y, std::vector<int>& color_vector);
};

#endif // PIXEL_H
