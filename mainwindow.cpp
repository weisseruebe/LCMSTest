#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageFileName = new QString("/Users/andreasrettig/Desktop/basICColor_CM-Ampel.jpeg");
    ui->label->setPixmap(QPixmap::fromImage(*convert(new QImage(*imageFileName),
                                                     cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r"),
                                                     cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r")))
                         );

}

QImage* MainWindow::convert(QImage* inImage, cmsHPROFILE inProfile, cmsHPROFILE outProfile){

    cmsHTRANSFORM hTransform;
    int w = inImage->width();
    int h = inImage->height();

    hTransform = cmsCreateTransform(inProfile,
                                    TYPE_RGB_8,
                                    outProfile,
                                    TYPE_RGB_8,
                                    INTENT_RELATIVE_COLORIMETRIC, 0);

    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);

    uchar *rgbOutTemp = new uchar[(w * h) * 3];
    uchar *rgbInTemp  = new uchar[(w * h) * 3];

    int j = 0;
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            QColor col = inImage->pixel(x, y);
            rgbInTemp[j] = col.red();
            rgbInTemp[j + 1] = col.green();
            rgbInTemp[j + 2] = col.blue();
            j += 3;
        }
    }

    cmsDoTransform(hTransform,rgbInTemp,rgbOutTemp,w*h);
    cmsDeleteTransform(hTransform);

    QImage* imout = new QImage(w, h, QImage::Format_RGB32);
    int s = 0;
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            QColor color(rgbOutTemp[s], rgbOutTemp[s+1], rgbOutTemp[s+2]);
            imout->setPixel(x, y, color.rgb());
            s+=3;
        }
    }
    return imout;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.icc)"));
    inProfileName = new QString(fileName);
    update();
}

void MainWindow::update(){
    ui->label->setPixmap(QPixmap::fromImage(*convert(new QImage(*imageFileName),
                                                     cmsOpenProfileFromFile(inProfileName->toAscii(), "r"),
                                                     cmsOpenProfileFromFile("/System/Library/ColorSync/Profiles/sRGB Profile.icc", "r")))
                         );
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.jpg)"));
    imageFileName = new QString(fileName);
    update();
}
