/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actF_OpenFile;
    QAction *actF_SaveFile;
    QAction *actF_SaveFileAsCopy;
    QAction *actF_Reload;
    QAction *actH_Greetings;
    QWidget *centralwidget;
    QLabel *showImg;
    QTabWidget *chooseFuncWidget;
    QWidget *funcCamera;
    QFrame *frame;
    QPushButton *pBtnC_CameraSwitch;
    QPushButton *pBtnC_CaptureFrame;
    QPushButton *pBtnC_ConductDetection;
    QPushButton *pBtnC_MethodSwitch;
    QScrollArea *sArea_CameraCapture;
    QWidget *scrollAreaWidgetContents;
    QGroupBox *groupBox;
    QFrame *frame_2;
    QRadioButton *radBtnR_ResolutionRatio1280x720;
    QRadioButton *radBtnR_ResolutionRatio1440x810;
    QRadioButton *radBtnR_ResolutionRatio1600x900;
    QRadioButton *radBtnR_ResolutionRatio1760x990;
    QRadioButton *radBtnR_ResolutionRatio1920x1080;
    QGroupBox *groupBox_2;
    QFrame *frame_3;
    QRadioButton *radBtnE_edgeDetection_Roberts;
    QRadioButton *radBtnE_edgeDetection_Sobel;
    QRadioButton *radBtnE_edgeDetection_Prewitt;
    QRadioButton *radBtnE_edgeDetection_Laplacian;
    QGroupBox *groupBox_3;
    QFrame *frame_4;
    QRadioButton *radBtnI_ImageSplit_DoubleHill;
    QRadioButton *radBtnI_ImageSplit_AdaptiveThreshold;
    QRadioButton *radBtnI_ImageSplit_Iteration;
    QGroupBox *groupBox_4;
    QFrame *frame_5;
    QRadioButton *radBtnZ_ZoneSegmentation_Skin;
    QWidget *tab_2;
    QFrame *frame_6;
    QLabel *label_ShowFaceCount;
    QMenuBar *menubar;
    QMenu *File;
    QMenu *Help;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 540);
        actF_OpenFile = new QAction(MainWindow);
        actF_OpenFile->setObjectName("actF_OpenFile");
        actF_SaveFile = new QAction(MainWindow);
        actF_SaveFile->setObjectName("actF_SaveFile");
        actF_SaveFileAsCopy = new QAction(MainWindow);
        actF_SaveFileAsCopy->setObjectName("actF_SaveFileAsCopy");
        actF_Reload = new QAction(MainWindow);
        actF_Reload->setObjectName("actF_Reload");
        actH_Greetings = new QAction(MainWindow);
        actH_Greetings->setObjectName("actH_Greetings");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        showImg = new QLabel(centralwidget);
        showImg->setObjectName("showImg");
        showImg->setGeometry(QRect(15, 10, 865, 485));
        chooseFuncWidget = new QTabWidget(centralwidget);
        chooseFuncWidget->setObjectName("chooseFuncWidget");
        chooseFuncWidget->setGeometry(QRect(950, 10, 300, 490));
        funcCamera = new QWidget();
        funcCamera->setObjectName("funcCamera");
        frame = new QFrame(funcCamera);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 20, 260, 121));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pBtnC_CameraSwitch = new QPushButton(frame);
        pBtnC_CameraSwitch->setObjectName("pBtnC_CameraSwitch");
        pBtnC_CameraSwitch->setGeometry(QRect(20, 10, 100, 20));
        pBtnC_CaptureFrame = new QPushButton(frame);
        pBtnC_CaptureFrame->setObjectName("pBtnC_CaptureFrame");
        pBtnC_CaptureFrame->setGeometry(QRect(150, 10, 100, 20));
        pBtnC_ConductDetection = new QPushButton(frame);
        pBtnC_ConductDetection->setObjectName("pBtnC_ConductDetection");
        pBtnC_ConductDetection->setGeometry(QRect(20, 70, 100, 20));
        pBtnC_MethodSwitch = new QPushButton(frame);
        pBtnC_MethodSwitch->setObjectName("pBtnC_MethodSwitch");
        pBtnC_MethodSwitch->setGeometry(QRect(160, 60, 80, 40));
        sArea_CameraCapture = new QScrollArea(funcCamera);
        sArea_CameraCapture->setObjectName("sArea_CameraCapture");
        sArea_CameraCapture->setGeometry(QRect(20, 150, 260, 300));
        sArea_CameraCapture->setMinimumSize(QSize(260, 300));
        sArea_CameraCapture->setMaximumSize(QSize(260, 300));
        sArea_CameraCapture->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 248, 800));
        scrollAreaWidgetContents->setMinimumSize(QSize(0, 800));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(5, 10, 240, 180));
        QFont font;
        font.setPointSize(12);
        groupBox->setFont(font);
        frame_2 = new QFrame(groupBox);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(10, 40, 220, 120));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        radBtnR_ResolutionRatio1280x720 = new QRadioButton(frame_2);
        radBtnR_ResolutionRatio1280x720->setObjectName("radBtnR_ResolutionRatio1280x720");
        radBtnR_ResolutionRatio1280x720->setGeometry(QRect(10, 10, 80, 20));
        QFont font1;
        font1.setPointSize(9);
        radBtnR_ResolutionRatio1280x720->setFont(font1);
        radBtnR_ResolutionRatio1280x720->setChecked(true);
        radBtnR_ResolutionRatio1440x810 = new QRadioButton(frame_2);
        radBtnR_ResolutionRatio1440x810->setObjectName("radBtnR_ResolutionRatio1440x810");
        radBtnR_ResolutionRatio1440x810->setGeometry(QRect(135, 10, 80, 20));
        radBtnR_ResolutionRatio1440x810->setFont(font1);
        radBtnR_ResolutionRatio1600x900 = new QRadioButton(frame_2);
        radBtnR_ResolutionRatio1600x900->setObjectName("radBtnR_ResolutionRatio1600x900");
        radBtnR_ResolutionRatio1600x900->setGeometry(QRect(10, 40, 80, 20));
        radBtnR_ResolutionRatio1600x900->setFont(font1);
        radBtnR_ResolutionRatio1760x990 = new QRadioButton(frame_2);
        radBtnR_ResolutionRatio1760x990->setObjectName("radBtnR_ResolutionRatio1760x990");
        radBtnR_ResolutionRatio1760x990->setGeometry(QRect(135, 40, 80, 20));
        radBtnR_ResolutionRatio1760x990->setFont(font1);
        radBtnR_ResolutionRatio1920x1080 = new QRadioButton(frame_2);
        radBtnR_ResolutionRatio1920x1080->setObjectName("radBtnR_ResolutionRatio1920x1080");
        radBtnR_ResolutionRatio1920x1080->setGeometry(QRect(10, 70, 90, 20));
        radBtnR_ResolutionRatio1920x1080->setFont(font1);
        radBtnR_ResolutionRatio1920x1080->setChecked(false);
        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(5, 200, 240, 180));
        groupBox_2->setFont(font);
        frame_3 = new QFrame(groupBox_2);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(10, 40, 220, 120));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        radBtnE_edgeDetection_Roberts = new QRadioButton(frame_3);
        radBtnE_edgeDetection_Roberts->setObjectName("radBtnE_edgeDetection_Roberts");
        radBtnE_edgeDetection_Roberts->setGeometry(QRect(10, 10, 80, 20));
        radBtnE_edgeDetection_Roberts->setFont(font1);
        radBtnE_edgeDetection_Sobel = new QRadioButton(frame_3);
        radBtnE_edgeDetection_Sobel->setObjectName("radBtnE_edgeDetection_Sobel");
        radBtnE_edgeDetection_Sobel->setGeometry(QRect(135, 10, 73, 20));
        radBtnE_edgeDetection_Sobel->setFont(font1);
        radBtnE_edgeDetection_Sobel->setChecked(true);
        radBtnE_edgeDetection_Prewitt = new QRadioButton(frame_3);
        radBtnE_edgeDetection_Prewitt->setObjectName("radBtnE_edgeDetection_Prewitt");
        radBtnE_edgeDetection_Prewitt->setGeometry(QRect(10, 90, 73, 20));
        radBtnE_edgeDetection_Prewitt->setFont(font1);
        radBtnE_edgeDetection_Laplacian = new QRadioButton(frame_3);
        radBtnE_edgeDetection_Laplacian->setObjectName("radBtnE_edgeDetection_Laplacian");
        radBtnE_edgeDetection_Laplacian->setGeometry(QRect(135, 90, 80, 20));
        radBtnE_edgeDetection_Laplacian->setFont(font1);
        groupBox_3 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(5, 390, 240, 180));
        groupBox_3->setFont(font);
        frame_4 = new QFrame(groupBox_3);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(10, 40, 220, 120));
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        radBtnI_ImageSplit_DoubleHill = new QRadioButton(frame_4);
        radBtnI_ImageSplit_DoubleHill->setObjectName("radBtnI_ImageSplit_DoubleHill");
        radBtnI_ImageSplit_DoubleHill->setGeometry(QRect(10, 10, 80, 20));
        radBtnI_ImageSplit_DoubleHill->setFont(font1);
        radBtnI_ImageSplit_AdaptiveThreshold = new QRadioButton(frame_4);
        radBtnI_ImageSplit_AdaptiveThreshold->setObjectName("radBtnI_ImageSplit_AdaptiveThreshold");
        radBtnI_ImageSplit_AdaptiveThreshold->setGeometry(QRect(10, 45, 80, 20));
        radBtnI_ImageSplit_AdaptiveThreshold->setFont(font1);
        radBtnI_ImageSplit_AdaptiveThreshold->setChecked(true);
        radBtnI_ImageSplit_Iteration = new QRadioButton(frame_4);
        radBtnI_ImageSplit_Iteration->setObjectName("radBtnI_ImageSplit_Iteration");
        radBtnI_ImageSplit_Iteration->setGeometry(QRect(10, 80, 80, 20));
        radBtnI_ImageSplit_Iteration->setFont(font1);
        groupBox_4 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(5, 580, 240, 180));
        groupBox_4->setFont(font);
        frame_5 = new QFrame(groupBox_4);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(10, 40, 220, 120));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        radBtnZ_ZoneSegmentation_Skin = new QRadioButton(frame_5);
        radBtnZ_ZoneSegmentation_Skin->setObjectName("radBtnZ_ZoneSegmentation_Skin");
        radBtnZ_ZoneSegmentation_Skin->setGeometry(QRect(20, 20, 80, 20));
        radBtnZ_ZoneSegmentation_Skin->setFont(font1);
        radBtnZ_ZoneSegmentation_Skin->setChecked(true);
        sArea_CameraCapture->setWidget(scrollAreaWidgetContents);
        chooseFuncWidget->addTab(funcCamera, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        frame_6 = new QFrame(tab_2);
        frame_6->setObjectName("frame_6");
        frame_6->setGeometry(QRect(10, 10, 280, 120));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        label_ShowFaceCount = new QLabel(frame_6);
        label_ShowFaceCount->setObjectName("label_ShowFaceCount");
        label_ShowFaceCount->setGeometry(QRect(20, 54, 240, 12));
        chooseFuncWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 17));
        File = new QMenu(menubar);
        File->setObjectName("File");
        Help = new QMenu(menubar);
        Help->setObjectName("Help");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(File->menuAction());
        menubar->addAction(Help->menuAction());
        File->addAction(actF_OpenFile);
        File->addSeparator();
        File->addAction(actF_SaveFile);
        File->addAction(actF_SaveFileAsCopy);
        File->addAction(actF_Reload);
        Help->addAction(actH_Greetings);

        retranslateUi(MainWindow);

        chooseFuncWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actF_OpenFile->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        actF_SaveFile->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\346\226\207\344\273\266", nullptr));
        actF_SaveFileAsCopy->setText(QCoreApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", nullptr));
        actF_Reload->setText(QCoreApplication::translate("MainWindow", "\351\207\215\350\275\275\346\226\207\344\273\266", nullptr));
        actH_Greetings->setText(QCoreApplication::translate("MainWindow", "\346\204\237\350\260\242\344\275\277\347\224\250", nullptr));
        showImg->setText(QString());
        pBtnC_CameraSwitch->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", nullptr));
        pBtnC_CaptureFrame->setText(QCoreApplication::translate("MainWindow", "\346\215\225\350\216\267\345\270\247", nullptr));
        pBtnC_ConductDetection->setText(QCoreApplication::translate("MainWindow", "\346\211\247\350\241\214\344\272\272\350\204\270\346\243\200\346\265\213", nullptr));
        pBtnC_MethodSwitch->setText(QCoreApplication::translate("MainWindow", "\350\275\254\346\215\242\346\226\271\346\263\225", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\350\260\203\346\225\264\346\221\204\345\203\217\345\244\264\347\252\227\345\217\243\345\210\206\350\276\250\347\216\207\357\274\232", nullptr));
        radBtnR_ResolutionRatio1280x720->setText(QCoreApplication::translate("MainWindow", "1280x720", nullptr));
        radBtnR_ResolutionRatio1440x810->setText(QCoreApplication::translate("MainWindow", "1440x810", nullptr));
        radBtnR_ResolutionRatio1600x900->setText(QCoreApplication::translate("MainWindow", "1600x900", nullptr));
        radBtnR_ResolutionRatio1760x990->setText(QCoreApplication::translate("MainWindow", "1760x990", nullptr));
        radBtnR_ResolutionRatio1920x1080->setText(QCoreApplication::translate("MainWindow", "1920x1080", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\350\276\271\347\274\230\346\243\200\346\265\213\347\256\227\345\255\220\351\200\211\346\213\251:", nullptr));
        radBtnE_edgeDetection_Roberts->setText(QCoreApplication::translate("MainWindow", "Roberts\347\256\227\345\255\220", nullptr));
        radBtnE_edgeDetection_Sobel->setText(QCoreApplication::translate("MainWindow", "Sobel\347\256\227\345\255\220", nullptr));
        radBtnE_edgeDetection_Prewitt->setText(QCoreApplication::translate("MainWindow", "Prewitt\347\256\227\345\255\220", nullptr));
        radBtnE_edgeDetection_Laplacian->setText(QCoreApplication::translate("MainWindow", "Laplacian\347\256\227\345\255\220", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\351\230\210\345\200\274\345\210\206\345\211\262\347\256\227\346\263\225\351\200\211\346\213\251\357\274\232", nullptr));
        radBtnI_ImageSplit_DoubleHill->setText(QCoreApplication::translate("MainWindow", "\347\233\264\346\226\271\345\233\276\345\217\214\345\263\260", nullptr));
        radBtnI_ImageSplit_AdaptiveThreshold->setText(QCoreApplication::translate("MainWindow", "\350\207\252\351\200\202\345\272\224\351\230\210\345\200\274", nullptr));
        radBtnI_ImageSplit_Iteration->setText(QCoreApplication::translate("MainWindow", "\350\277\255\344\273\243\351\230\210\345\200\274", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\345\214\272\345\237\237\345\210\206\345\211\262\347\256\227\346\263\225\351\200\211\346\213\251:", nullptr));
        radBtnZ_ZoneSegmentation_Skin->setText(QCoreApplication::translate("MainWindow", "\350\202\244\350\211\262\345\210\206\345\211\262", nullptr));
        chooseFuncWidget->setTabText(chooseFuncWidget->indexOf(funcCamera), QCoreApplication::translate("MainWindow", "\345\237\272\344\272\216\346\221\204\345\203\217\345\244\264\347\232\204\350\257\206\345\210\253", nullptr));
        label_ShowFaceCount->setText(QCoreApplication::translate("MainWindow", "\350\257\267\350\277\233\350\241\214\344\272\272\350\204\270\346\243\200\346\265\213\357\274\201", nullptr));
        chooseFuncWidget->setTabText(chooseFuncWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        File->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        Help->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
