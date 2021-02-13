#include <QPainter>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sphere.h"
#include "light.h"
#include "vectors_math.h"



struct Pixel {
    int x;
    int y;
    int color[3];
};

const double Vw = 1;
const double Vh = 1;
int Cw;
int Ch;
int step;
std::vector<Pixel> PixelMap;
const double d = 1;
const double Vz = d;
const int RECURSIVE_DEPTH = 3; // Depth of mirroring recusrion
std::vector<int> BG_COLOR = {0, 0, 0};


Light l1 = {Light::type::ambient, {0.2, 0.2, 0.2}};
Light l2 = {Light::type::point, {1, 1, 1}, {0, 2, 3}};
//Light l3 = {Light::type::directional, 0.2, {}, {1 , 4, 4}};
std::vector<Light> lights = {l1, l2};

Sphere* s1 = new Sphere {{0, -1, 3}, 1, {255, 10, 10}, 500, 0.2};
Sphere* s2 = new Sphere {{2, 0, 4}, 1, {10, 10, 255}, 10, 0.3};
Sphere* s3 = new Sphere {{-2, 0, 4}, 1, {10, 255, 10}, 100, 0.4};
Sphere* s4 = new Sphere {{0, -5001, 0}, 5000, {255, 255, 0}, 10, 0.5};
//Sphere* sLight = new Sphere {{l2.position[0], l2.position[1] - 0.5, l2.position[2]}, 0.1, {255, 255, 0}};
std::vector<Object*> objects = {s1, s2, s3, s4};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage image(ui->widget->width(), ui->widget->height(), QImage::Format_ARGB32_Premultiplied);
    image.save(PICTURE_PATH);
    Cw = ui->widget->width();
    Ch = ui->widget->height();
//    draw_spheres();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QImage img(PICTURE_PATH);
    QPainter painter(this);
    painter.drawImage(ui->widget->geometry().x(),
                      ui->widget->geometry().y(),
                      img);
}

std::vector<double> CanvasToViewport(double x, double y) {
    return {x * Vw / Cw, y * Vh / Ch, d};
}

void ClosestIntersection (const std::vector<double>& O,
                          const std::vector<double>& D,
                          const double& t_min,
                          const double& t_max,
                          double& closest_t,
                          Object*& closest_object) {
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
                        const double& s) {
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
            ClosestIntersection(P, L, 1E-3, t_max, shadow_t, closest_shadow_object);
            if (closest_shadow_object != nullptr) {
                continue;
            }

            // Diffusual
            double n_dot_l = VectorScalarMult(N, L);
            if (n_dot_l > 0) {
                i[0] += light.color_intensity[0] * n_dot_l / (VectorLength(N) * VectorLength(L));
                i[1] += light.color_intensity[1] * n_dot_l / (VectorLength(N) * VectorLength(L));
                i[2] += light.color_intensity[2] * n_dot_l / (VectorLength(N) * VectorLength(L));
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

    return i;
}

std::vector<int> TraceRay(const std::vector<double>& O,
                          const std::vector<double>& D,
                          double t_min,
                          double t_max,
                          int depth) {
    double closest_t = std::numeric_limits<double>::max();
    Object* closest_object = nullptr;

    ClosestIntersection(O, D, t_min, t_max, closest_t, closest_object);

    if (closest_object == nullptr) {
        return BG_COLOR;
    }

    std::vector<double> P = VectorSum(O, VectorMult(D, closest_t));
    std::vector<double> N = VectorDiff(P, closest_object->getCenter());
    N = {N[0] / VectorLength(N),
         N[1] / VectorLength(N),
         N[2] / VectorLength(N)};
    std::vector<int> local_color = VectorMult(closest_object->getColor(),
                                              ComputeLightning(P,
                                                               N,
                                                               VectorMult(D, -1),
                                                               closest_object->getSpecular()));

    double r = closest_object->getReflective();
    if (depth <= 0 || r <= 0) {
        return local_color;
    }

    std::vector<double> R = ReflectRay(VectorMult(D, -1), N);
    std::vector<int> reflected_color = TraceRay(P, R, 1E-3, std::numeric_limits<double>::max(), depth - 1);

    return VectorSum(VectorMult(local_color, 1 - r), VectorMult(reflected_color, r));
}


void MainWindow::draw_spheres(char spinAxis) {
    QImage img(PICTURE_PATH);
    std::vector<double> O{ui->OxInput->value(), ui->OyInput->value(), ui->OzInput->value()};
    std::vector<std::vector<double>> MatrixSpin;
    switch (spinAxis) {
        case ('x') :
            MatrixSpin = XSpinMatrixCalculate(-ui->OxAngleInput->value());
            break;
        case ('y') :
            MatrixSpin = YSpinMatrixCalculate(-ui->OyAngleInput->value());
            break;
        case ('z') :
            MatrixSpin = ZSpinMatrixCalculate(-ui->OzAngleInput->value());
            break;
        default:
            break;
    }
    for (int x = -Cw / 2; x < Cw / 2; x++) {
        for (int y = -Ch / 2; y < Ch / 2; y++) {
            std::vector<double> D = CanvasToViewport(x, y);
            if (spinAxis != '0') {D = MatrixOnVectorMult(MatrixSpin, D);}
//            std::vector<double> D = CanvasToViewport(x, y);
            std::vector<int> color = TraceRay(O, D, 1, std::numeric_limits<double>::max(), RECURSIVE_DEPTH);
            if (color[0] != 0 || color[1] != 0 || color[2] != 0) {
                std::cout << color[0] << " " << color[1] << " " << color[2] << std::endl;
            }
            img.setPixelColor(Cw / 2 + x,
                              Ch / 2 - y,
                              QColor(color[0], color[1], color[2]));
        }
    }

    img.save(PICTURE_PATH);
    update();

    // Just Testing
//    lights[1].setPosition({lights[1].position[0],
//                           lights[1].position[1],
//                           lights[1].position[2] - 0.3});
//    objects[4]->setCenter({lights[1].position[0],
//                           lights[1].position[1] - 0.5,
//                           lights[1].position[2]});
}

void MainWindow::on_pushButton_clicked()
{
    draw_spheres();
}

void MainWindow::on_pushXSpin_clicked()
{
    draw_spheres('x');
}

void MainWindow::on_pushYSpin_clicked()
{
    draw_spheres('y');
}

void MainWindow::on_pushZSpin_clicked()
{
    draw_spheres('z');
}
