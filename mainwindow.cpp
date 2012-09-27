#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lcms2.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cmsHPROFILE hInProfile, hOutProfile;
    cmsHTRANSFORM hTransform;

    hInProfile  = cmsOpenProfileFromFile("/Users/andreasrettig/Desktop/ampel.icc", "r");
    hOutProfile = cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r");
    hTransform = cmsCreateTransform(hInProfile,
                                    TYPE_BGR_8,
                                    hOutProfile,
                                    TYPE_BGR_8,
                                    INTENT_SATURATION, 0);
    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);
    QImage* image = new QImage("/Users/andreasrettig/Desktop/basICColor_CM-Ampel.jpeg");

    uchar *rgb_temp = new uchar[(image->width() * image->height()) * 3];

//    for (int i=0;i< 200; i++){
//        uchar* scanLine = image->scanLine(i);
//        cmsDoTransform(hTransform,scanLine,scanLine,image->width()*4);
//    }


    int j = 0;
    uchar *rgbIn_temp = new uchar[(image->width() * image->height()) * 3];

    // default
    for(int y = 0; y < image->height(); y++){
        for(int x = 0; x < image->width(); x++){
            QColor fp = image->pixel(x, y);
            rgbIn_temp[j] = fp.blue();
            rgbIn_temp[j + 1] = fp.green();
            rgbIn_temp[j + 2] = fp.red();
            j += 3;
        }
    }

    cmsDoTransform(hTransform,rgbIn_temp,rgb_temp,image->width()*image->height());

    QImage* imout = new QImage(image->width(), image->height(), QImage::Format_RGB32);

    int s = 0;
    for(int h = 0; h < imout->height(); h++){
        for(int w = 0; w < imout->width(); w++){
            QColor color(rgb_temp[s], rgb_temp[s+1], rgb_temp[s+2]);
            imout->setPixel(w, h, color.rgb());
            s+=3;
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(*imout));

}

MainWindow::~MainWindow()
{
    delete ui;
}
