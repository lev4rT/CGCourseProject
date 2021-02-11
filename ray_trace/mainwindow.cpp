#include <QPainter>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

const double Vw = 1;
const double Vh = 1;
int Cw;
int Ch;
const double d = 1;
const double Vz = d;
std::vector<int> BG_COLOR = {255, 255, 255};


struct Sphere {
    std::vector<double> center;
    double r;
    std::vector<int> color;
};

Sphere s1 = {{0, -1, 3}, 1, {255, 0, 0}};
Sphere s2 = {{2, 0, 4}, 1, {0, 0, 255}};
Sphere s3 = {{-2, 0, 4}, 1, {0, 255, 0}};
Sphere s4 = {{0, -5001, 0}, 5000, {255, 255, 0}};
std::vector<Sphere> objects = {s1, s2, s3, s4};


struct Light {
    enum type {
        ambient = 0,
        point,
        directional
    };

    type t;
    double intensity;
    std::vector<double> position;
    std::vector<double> direction;
};

Light l1 = {Light::type::ambient, 0.2};
Light l2 = {Light::type::point, 0.6, {-1, 1, 0}};
Light l3 = {Light::type::directional, 0.2, {}, {1 , 4, 4}};
std::vector<Light> lights = {l1, l2, l3};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage image(ui->widget->width(), ui->widget->height(), QImage::Format_ARGB32_Premultiplied);
    image.save(PICTURE_PATH);
    Cw = ui->widget->width();
    Ch = ui->widget->height();
    draw_spheres();
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


double ScalarMult(const std::vector<double>& U, const std::vector<double>& V) {
    return U[0] * V[0] + U[1] * V[1] + U[2] * V[2];
}


std::vector<double> IntersectRaySphere(const std::vector<double>& O, const std::vector<double>& D, const Sphere& sphere) {
    std::vector<double> OC = {O[0] - sphere.center[0],
                              O[1] - sphere.center[1],
                              O[2] - sphere.center[2]};

    double k1 = ScalarMult(D, D);
    double k2 = 2 * ScalarMult(OC, D);
    double k3 = ScalarMult(OC, OC) - sphere.r * sphere.r;

    double discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0) {
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    }

    double t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
    double t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
    return {t1, t2};
}


double VectorLength(const std::vector<double>& v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

std::vector<double> VectorSum(const std::vector<double>& u, const std::vector<double>& v) {
    return {v[0] + u[0], v[1] + u[1], v[2] + u[2]};
}

std::vector<double> VectorMult(const std::vector<double>& u, const double& v) {
    return {u[0] * v, u[1] * v, u[2] * v};
}

std::vector<int> VectorMult(const std::vector<int>& u, const double& v) {
    return {(int) round(u[0] * v), (int) round(u[1] * v), (int) round(u[2] * v)};
}


std::vector<double> VectorDiff(const std::vector<double>& u, const std::vector<double>& v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

double ComputeLightning(const std::vector<double>& P, const std::vector<double>& N) {
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

            double n_dot_l = ScalarMult(N, L);
            if (n_dot_l > 0) {
                i += light.intensity * n_dot_l / (VectorLength(N) * VectorLength(L));
            }
        }
    }

    return i;
}

std::vector<int> TraceRay(const std::vector<double>& O, const std::vector<double>& D, double t_min, double t_max) {
    double closest_t = std::numeric_limits<double>::max();
    Sphere* closest_sphere = nullptr;
    for (size_t i = 0; i < objects.size(); i++) {
        std::vector<double> intersections = IntersectRaySphere(O, D, objects[i]);
        if (intersections[0] > t_min && intersections[0] < t_max && intersections[0] < closest_t) {
            closest_t = intersections[0];
            closest_sphere = &objects[i];
        }
        if (intersections[1] > t_min && intersections[1] < t_max && intersections[1] < closest_t) {
            closest_t = intersections[1];
            closest_sphere = &objects[i];
        }
    }

    if (closest_sphere == nullptr) {
        return BG_COLOR;
    }

    std::vector<double> P = VectorSum(O, VectorMult(D, closest_t));
    std::vector<double> N = VectorDiff(P, closest_sphere->center);
    N = {N[0] / VectorLength(N),
         N[1] / VectorLength(N),
         N[2] / VectorLength(N)};

    return VectorMult(closest_sphere->color, ComputeLightning(P, N));
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
}
