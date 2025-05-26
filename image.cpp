#include "image.h"

Image::Image() {}

Mat Image::getCVimgOrigin(){
    return m_CVimg_Origin;
}
Mat Image::getCVimgTemp(){
    return m_CVimg_Temp;
}

QImage Image::getQimgOrigin(){
    return m_Qimg_Origin;
}
QImage Image::getQimgTemp(){
    return m_Qimg_Temp;
}
QString Image::getFileName(){
    return filename;
}
int Image::getScaledWidth(){
    return scaledWidth;
}
int Image::getScaledHeight(){
    return scaledHeight;
}

void Image::setScaledWidth(int width){
    scaledWidth = width;
}
void Image::setScaledHeight(int height){
    scaledHeight = height;
}
void Image::setCVimgOrigin(Mat src){
    m_CVimg_Origin = src.clone();
}
void Image::setCVimgTemp(Mat src){
    m_CVimg_Temp = src.clone();
}
void Image::setQimgOrigin(QImage src){
    m_Qimg_Origin = src.copy();
}
void Image::setQimgTemp(QImage src){
    m_Qimg_Temp = src.copy();
}
void Image::setFileName(QString inputString){
    filename = inputString;
}

void Image::cvtCV2Qimg(Mat src){
    Mat cvt;
    QImage dst;

    if (src.channels()==1){
        src.copyTo(cvt);
    }
    else if (src.channels()==3){
        cvtColor(src,cvt,COLOR_BGR2RGB);
    }
    else if (src.channels()==4){
        cvtColor(src,cvt,COLOR_BGRA2RGBA);
    }

    dst = QImage(cvt.data,cvt.cols,cvt.rows,cvt.step,
                (cvt.channels() == 3) ? QImage::Format_RGB888 :
                (cvt.channels() == 4) ? QImage::Format_RGBA8888 :
                QImage::Format_Grayscale8);

    m_Qimg_Temp = dst.copy();
}

void Image::resetElements(){

}

void Image::saveWithCount(Mat src,QString& saveDirPath,int Object){
    QString nameHead;
    QDir saveDir(saveDirPath);
    //若不存在则创建一个
    if (!saveDir.exists()){
        if (!saveDir.mkpath(".")){
            qWarning() << "无法创建目录:" << saveDirPath;
            return;
        }
    }

    //名字过滤器
    QStringList nameFilters;
    //过滤规则
    if (Object == PRE_PRODUCE){
        nameFilters << "imagePreP_*.png";
        nameHead = "imagePreP_";
    }
    else if (Object == EDGE_DETEC){
        nameFilters << "imageEdge_*.png";
        nameHead = "imageEdge_";
    }
    else if (Object == THRE_SEG){
        nameFilters << "imageThre_*.png";
        nameHead = "imageThre_";
    }
    else if (Object == ZONE_MARK){
        nameFilters << "imageZone_*.png";
        nameHead = "imageZone_";
    }
    //entryInfoList()：QDir 类的成员函数，用于获取目录中的条目（文件或子目录）的详细信息
    QFileInfoList files = saveDir.entryInfoList(nameFilters,QDir::Files);
    QFileInfoList filesToSave = files;

    int maxNumber = 0;
    //遍历从对应目录中提取的所有图像文件名
    for (const QFileInfo &fileInfo : filesToSave) {
        //去除文件拓展名
        QString baseName = fileInfo.baseName();
        //若命名规则符合
        if (baseName.startsWith(nameHead)){
            //取名字串的第六位开始的数(即数字编号)
            QString numStr = baseName.mid(10);
            //将numStr转化为int类型
            int currentNum = numStr.toInt();
            if (currentNum > maxNumber){
                maxNumber = currentNum;
            }
        }
    }

    int nextNum = maxNumber + 1;
    QString fileName;
    QString filePath;
    //防御性编程，防止程序其他部分已经创建了序号更大的图像而导致可能的覆盖
    if (Object==PRE_PRODUCE){
        do {
            fileName = QString("imagePreP_%1.jpg").arg(nextNum,4,10,QChar('0'));
            filePath = saveDir.filePath(fileName);
            nextNum++;
        }while (QFileInfo::exists(filePath));       //若已存在则继续生成
    }
    else if (Object==EDGE_DETEC){
        do {
            fileName = QString("imageEdge_%1.jpg").arg(nextNum,4,10,QChar('0'));
            filePath = saveDir.filePath(fileName);
            nextNum++;
        }while (QFileInfo::exists(filePath));       //若已存在则继续生成
    }
    else if (Object==THRE_SEG){
        do {
            fileName = QString("imageThre_%1.jpg").arg(nextNum,4,10,QChar('0'));
            filePath = saveDir.filePath(fileName);
            nextNum++;
        }while (QFileInfo::exists(filePath));       //若已存在则继续生成
    }
    else if (Object==ZONE_MARK){
        do {
            fileName = QString("imageZone_%1.jpg").arg(nextNum,4,10,QChar('0'));
            filePath = saveDir.filePath(fileName);
            nextNum++;
        }while (QFileInfo::exists(filePath));       //若已存在则继续生成
    }
    else if (Object==FACE_DETECT){
        do {
            fileName = QString("imageFace_%1.jpg").arg(nextNum,4,10,QChar('0'));
            filePath = saveDir.filePath(fileName);
            nextNum++;
        }while (QFileInfo::exists(filePath));
    }
    //保存图像
    cvtCV2Qimg(src);
    QImage dst = m_Qimg_Temp;
    if (!dst.save(filePath,"png")){
        qWarning() << "保存失败:" << filePath;
    } else {
        qDebug() << "保存成功:" << filePath;
    }
}
