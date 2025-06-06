QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chooseTraditionalDetectionTypeWidget.cpp \
    image.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    chooseTraditionalDetectionTypeWidget.h \
    image.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += E:\Learning\OpenCV_4.9.0-Bulid\install\include
LIBS += E:\Learning\OpenCV_4.9.0-Bulid\install\x64\mingw\bin\libopencv_*.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    haarcascade.qrc
