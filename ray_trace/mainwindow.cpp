#include <QPainter>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
#include <thread>
#include <QRegExpValidator>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "vectors_math.h"
#include "mythread.h"
#include "pixel.h"

std::vector<double> O;
std::vector<std::vector<double>> MatrixSpin;
double Vw = 1;
double Vh = 1;
double d = 1;
int Cw;
int Ch;
int step;
std::vector<std::vector<Pixel>> PixelMap;
int RECURSIVE_DEPTH = 3; // Depth of mirroring recusrion
std::vector<int> BG_COLOR = {0, 0, 0};


Light l1 = {Light::type::ambient, {0.2, 0.2, 0.2}};
Light l2 = {Light::type::point, {1, 1, 1}, {0, 0.7, 3}};
//Light l3 = {Light::type::directional, 0.2, {}, {1 , 4, 4}};
std::vector<Light> lights = {l1, l2};

Sphere* s1 = new Sphere {{0, -1, 3}, 1, {255, 10, 10}, 500, 0.2, 0.5};
Sphere* s2 = new Sphere {{2, 0, 4}, 1, {10, 10, 255}, 10, 0.3, 0.5};
Sphere* s3 = new Sphere {{-2, 0, 4}, 1, {10, 255, 10}, 100, 0.4, 0.5};
Sphere* s4 = new Sphere {{0, -5001, 0}, 5000, {255, 255, 0}, 1000, 0.1, 0.5};
Sphere* s5 = new Sphere {{0, 1, 4}, 1, {10, 255, 10}, 100, 0.4, 0.5};
Sphere* lightSphere = new Sphere {{l2.position[0], l2.position[1], l2.position[2]}, 0.1, {255, 0, 0}, 500, 0.2, 0.5, Object::type::sphereSouceColor};

//double A, double B,
//double C, double D,
//std::vector<int> co, double spec, double ref
Plane* p1 = new Plane {0, 1, 0, -1, {255, 255, 0}, 0, 0, 0, -5, 5, -5, 5, -5, 5};
//Sphere* sLight = new Sphere {{l2.position[0], l2.position[1] - 0.5, l2.position[2]}, 0.1, {255, 255, 0}};
std::vector<Object*> objects = {s1, s2, s3, s4, lightSphere};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")));
    ui->OxInput->setValidator(new QRegExpValidator(QRegExp("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")));
    ui->OyInput->setValidator(new QRegExpValidator(QRegExp("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")));
    ui->OzInput->setValidator(new QRegExpValidator(QRegExp("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")));
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


void MainWindow::draw_spheres(char spinAxis) {
    O = {ui->OxInput->text().toDouble(), ui->OyInput->text().toDouble(), ui->OzInput->text().toDouble()};
    switch (spinAxis) {
        case ('x') :
            MatrixSpin = XSpinMatrixCalculate(ui->OxAngleInput->text().toDouble());
            break;
        case ('y') :
            MatrixSpin = YSpinMatrixCalculate(-ui->OyAngleInput->text().toDouble());
            break;
        case ('z') :
            MatrixSpin = ZSpinMatrixCalculate(-ui->OzAngleInput->text().toDouble());
            break;
        default:        
            MatrixSpin = {};
            break;
    }


//    for (int x = -Cw / 2; x < Cw / 2; x++) {
//        for (int y = -Ch / 2; y < Ch / 2; y++) {
//            std::vector<double> D = CanvasToViewport(x, y);
//            if (spinAxis != '0') {D = MatrixOnVectorMult(MatrixSpin, D);}
//            std::vector<int> color = TraceRay(O, D, 1, std::numeric_limits<double>::max(), RECURSIVE_DEPTH);
//            if (color[0] != 0 || color[1] != 0 || color[2] != 0) {
//                std::cout << color[0] << " " << color[1] << " " << color[2] << std::endl;
//            }
//            img.setPixelColor(Cw / 2 + x,
//                              Ch / 2 - y,
//                              QColor(color[0], color[1], color[2]));
//        }
//    }

    const int threadsAmount = 16;
    std::vector<std::thread*> threads;
    std::cout << Cw << std::endl;
    for (int i = 0; i < threadsAmount; i++) {
        double xStart = Cw / threadsAmount * i - Cw / 2;
        double xEnd =   Cw / threadsAmount * (i + 1) - Cw / 2;
        std::thread* thread = new std::thread(CalculateRays, xStart, xEnd,
                                                             std::ref(Vw), std::ref(Vh),
                                                             std::ref(d),
                                                             std::ref(Cw), std::ref(Ch),
                                                             std::ref(MatrixSpin),
                                                             std::ref(O),
                                                             std::ref(PixelMap),
                                                             std::ref(objects),
                                                             std::ref(lights),
                                                             BG_COLOR,
                                                             RECURSIVE_DEPTH);
        threads.push_back(thread);
    }

    for (int i = 0; i < threadsAmount; i++) {
        threads[i]->join();
    }


    std::cout << Cw << std::endl;
    QImage img(PICTURE_PATH);
    for (const auto& vector : PixelMap) {
        for (const auto& pixel : vector) {
//            std::cout << "x: " << pixel.x << " y: " << pixel.y << std::endl;
            img.setPixelColor(pixel.x,
                              pixel.y,
                              pixel.color);
        }
    }
    img.save(PICTURE_PATH);
    update();
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
