#ifndef IMAGE_H
#define IMAGE_H

#include <QDir>
#include <QFileInfoList>
#include <QStringList>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

#define PRE_PRODUCE 0
#define EDGE_DETEC 1
#define THRE_SEG 2
#define ZONE_MARK 3
#define FACE_DETECT 4
#define MASK_GET 5

using namespace cv;

class Image{
private:
    //Variates
    QImage m_Qimg_Origin;
    QImage m_Qimg_Temp;
    Mat m_CVimg_Origin;
    Mat m_CVimg_Temp;

    QString filename;

    int scaledWidth;
    int scaledHeight;

public:
    //Set
    void setQimgOrigin(QImage qimg);
    void setQimgTemp(QImage qimg);
    void setCVimgOrigin(Mat cvimg);
    void setCVimgTemp(Mat cvimg);
    void setFileName(QString fileName);
    //Get
    QImage getQimgOrigin();
    QImage getQimgTemp();
    Mat getCVimgOrigin();
    Mat getCVimgTemp();
    QString getFileName();

    //Show
    void cvtCV2Qimg(Mat src);
    //resetElements
    void resetElements();
    //
    void saveWithCount(Mat src,QString& saveDirPath,int Object);

//构造函数
public:
    Image();
};

#endif // IMAGE_H
