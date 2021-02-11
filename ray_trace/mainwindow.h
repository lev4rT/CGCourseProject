#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define PICTURE_PATH "C:\\kursach\\test.png"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    void draw_spheres();
};
#endif // MAINWINDOW_H
