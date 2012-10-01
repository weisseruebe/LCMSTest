#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lcms2.h"
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage* convert(QImage* inImage, cmsHPROFILE inProfile, cmsHPROFILE outProfile);
    void update();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    QString* imageFileName;
    QString* inProfileName;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
