#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lcms2.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // QImage* image = new QImage("/Users/andreasrettig/Desktop/basICColor_CM-Ampel.jpeg");
    QImage* image = new QImage("/Users/andreasrettig/Desktop/helge/IMGP9322kl.jpg");

    cmsHPROFILE hInProfile, hOutProfile;
    cmsHTRANSFORM hTransform;

   //hInProfile  = cmsOpenProfileFromFile("/Users/andreasrettig/Desktop/ampel.icc", "r");
   hInProfile  = cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r");

    hOutProfile = cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r");
    hTransform = cmsCreateTransform(hInProfile,
                                    TYPE_RGB_8,
                                    hOutProfile,
                                    TYPE_RGB_8,
                                    INTENT_RELATIVE_COLORIMETRIC, 0);

    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);

    uchar *rgbOutTemp = new uchar[(image->width() * image->height()) * 3];
    uchar *rgbInTemp = new uchar[(image->width() * image->height()) * 3];

    int j = 0;
    for(int y = 0; y < image->height(); y++){
        for(int x = 0; x < image->width(); x++){
            QColor col = image->pixel(x, y);
            rgbInTemp[j] = col.red();
            rgbInTemp[j + 1] = col.green();
            rgbInTemp[j + 2] = col.blue();
            j += 3;
        }
    }

    cmsDoTransform(hTransform,rgbInTemp,rgbOutTemp,image->width()*image->height());

    QImage* imout = new QImage(image->width(), image->height(), QImage::Format_RGB32);
    int s = 0;
    for(int h = 0; h < imout->height(); h++){
        for(int w = 0; w < imout->width(); w++){
            QColor color(rgbOutTemp[s], rgbOutTemp[s+1], rgbOutTemp[s+2]);
            imout->setPixel(w, h, color.rgb());
            s+=3;
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(*imout));
    imout->save("/Users/andreasrettig/Desktop/saved.png","PNG",100);

}

MainWindow::~MainWindow()
{
    delete ui;
}
