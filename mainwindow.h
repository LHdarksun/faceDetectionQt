#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QRadioButton>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "image.h"

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    //Func

private:
//Variates
    int countDown;              //倒计时
    int ImgShowWindowLength;    //摄像头窗口长
    int ImgShowWindowWidth;     //摄像头窗口宽

    int edgeDetectionType;      //边缘检测的算法种类
    int thresholdSegmentationType;   //阈值分割的算法种类
    int zoneSegmentationType;        //区域标记的算法种类

    bool isFrame;               //判断是否为帧
    bool isImshowInitialized;   //判断摄像头窗口是否初始化成功
    bool isMachineLearning;     //判断人脸识别方法
    Image Img;                  //打开图像对象
    Image Frame;                //截取帧图像对象
    VideoCapture capture;       //摄像机对象

    CascadeClassifier faceCascade, eyeCascade, noseCascade, mouthCascade;

    QString imageRootPath;      //图像根目录
    QString imageProducedPath;      //经过一系列人脸检测处理后图像的存储路径
    QString imageFacePath;          //检测出来的人脸的保存地址
    QString imageFaceWithOrgansPath;
    QString imageFaceROIPath;
    QString imageMaskPath;
    QString xmlEyePath;
    QString xmlFacePath;
    QString xmlMouthPath;
    QString xmlNosePath;

    QList<QAction*> actionsF;   //指向行动对象
    QList<QAction*> actsF;      //防止actionsF被拆散
    QList<QPushButton*> pBtnCamera;   //指向按钮对象
    QList<QPushButton*> pBtnC;        //防止pBtnCamera被拆散
    QList<QRadioButton*> radBtnResolution;      //指向摄像头窗口分辨率
    QList<QRadioButton*> radBtnR;               //防止radBtnResolution被拆散
    QList<QRadioButton*> radBtnEdgeDetection;   //指向边缘检测算子选择按钮
    QList<QRadioButton*> radBtnE;               //防止radBtnEdgeDetection被拆散
    QList<QRadioButton*> radBtnThresholdSegmentation;     //指向图像分割算法按钮
    QList<QRadioButton*> radBtnT;               //防止radBtnZoneMarking被拆散
    QList<QRadioButton*> radBtnZoneSegmentation;
    QList<QRadioButton*> radBtnZ;
    QMessageBox *msgBoxInfo001; //倒计时提示弹窗对象
    QTimer *cameraTimer;        //刷新摄像头视频帧的计时器
    QTimer *countDownTimer;     //倒计时计时器

//Funcs
    void ImgShow(Mat src);
    void disableItemsActF(bool isCopy);
    void enableItems();
    void updateCameraFrame();
    void updateCameraButton();
    void updateCountDown();
    void showCameraPop();
    void updateFaceDetectionType();

    void chooseResolutionRatioType();
    void chooseEdgeDetectionType();
    void chooseThresholdSegmentationType();
    void chooseZoneSegmentationType();
    //new
    void chooseFaceDetectionType();

    Mat lightCompensation(Mat src);
    Mat histogramEqualization(Mat src);
    Mat edgeDetection(Mat src);
    Mat thresholdSegmentation(Mat src);
    Mat zoneSegmentation(Mat src);
    //new
    void faceDetectionMachineLearning(Mat src);
    void faceDetectionTraditional(Mat src);

    QString getHaarPath(const QString &fileName);

private slots:
    //帮助向导等文档
    void on_actH_Greeting_triggered();
    //图像的加载，存储
    void on_actF_OpenFile_triggered();
    void on_actF_SaveFile_triggered();
    void on_actF_SaveFileAsCopy_triggered();
    void on_actF_Reload_triggered();
    //摄像头
    void on_pBtnC_CameraSwitch_clicked();
    void on_pBtnC_CaptureFrame_clicked();
    //人脸识别
    void on_pBtnC_ConductDetection_clicked();
    void on_pBtnC_MethodSwitch_clicked();
};
#endif // MAINWINDOW_H
