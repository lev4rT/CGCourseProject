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


const double Vw = 1;
const double Vh = 1;
int Cw;
int Ch;
const double d = 1;
const double Vz = d;
std::vector<int> BG_COLOR = {255, 255, 255};


Light l1 = {Light::type::ambient, 0.2};
Light l2 = {Light::type::point, 0.6, {2, 1, 0}};
Light l3 = {Light::type::directional, 0.2, {}, {1 , 4, 4}};
std::vector<Light> lights = {l1, l2, l3};

Sphere* s1 = new Sphere {{0, -1, 3}, 1, {255, 0, 0}, 500};
Sphere* s2 = new Sphere {{2, 0, 4}, 1, {0, 0, 255}, 10};
Sphere* s3 = new Sphere {{-2, 0, 4}, 1, {0, 255, 0}, 100};
Sphere* s4 = new Sphere {{0, -5001, 0}, 5000, {255, 255, 0}, 10};
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

double ComputeLightning(const std::vector<double>& P,
                        const std::vector<double>& N,
                        const std::vector<double>& V,
                        const double& s) {
    double i = 0;
    std::vector<double> L;
    for (const auto& light : lights) {
        if (light.t == Light::type::ambient) {
            i += light.intensity;
        } else {
            if (light.t == Light::type::point) {
                L = {light.position[0] - P[0],
                     light.position[1] - P[1],
                     light.position[2] - P[2]};
            } else {
                L =light.direction;
            }

            // Diffusual
            double n_dot_l = VectorScalarMult(N, L);
            if (n_dot_l > 0) {
                i += light.intensity * n_dot_l / (VectorLength(N) * VectorLength(L));
            }

            // Mirroring
            if (fabs(s + 1) > 1E-6) {
                std::vector<double> R = VectorDiff(VectorMult(VectorMult(N, 2), VectorScalarMult(N, L)), L);
                double r_dot_v = VectorScalarMult(R, V);
                if (r_dot_v > 0) {
                    i += light.intensity * pow(r_dot_v / (VectorLength(R) * VectorLength(V)), s);
                }
            }
        }
    }

    return i;
}

std::vector<int> TraceRay(const std::vector<double>& O, const std::vector<double>& D, double t_min, double t_max) {
    double closest_t = std::numeric_limits<double>::max();
    Object* closest_object = nullptr;
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

    if (closest_object == nullptr) {
        return BG_COLOR;
    }

    std::vector<double> P = VectorSum(O, VectorMult(D, closest_t));
    std::vector<double> N = VectorDiff(P, closest_object->getCenter());
    N = {N[0] / VectorLength(N),
         N[1] / VectorLength(N),
         N[2] / VectorLength(N)};

    return VectorMult(closest_object->getColor(), ComputeLightning(P, N, VectorMult(D, -1), closest_object->getSpecular()));
//    return closest_sphere->color; Old shit, Man
}

void MainWindow::draw_spheres() {
    QImage img(PICTURE_PATH);
    std::vector<double> O{0, 0, 0};
    for (int x = -Cw / 2; x < Cw / 2; x++) {
        for (int y = -Ch / 2; y < Ch / 2; y++) {
            std::vector<double> D = CanvasToViewport(x, y);
            std::vector<int> color = TraceRay(O, D, 1, std::numeric_limits<double>::max());
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
    lights[1].setPosition({lights[1].position[0],
                           lights[1].position[1],
                           lights[1].position[2] - 0.3});
//    objects[4]->setCenter({lights[1].position[0],
//                           lights[1].position[1] - 0.5,
//                           lights[1].position[2]});
}

void MainWindow::on_pushButton_clicked()
{
    draw_spheres();
}
