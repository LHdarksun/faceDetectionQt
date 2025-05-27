#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <direct.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char buffer[1024];
    getcwd(buffer,sizeof(buffer));
    qDebug() << "Current Directory:" << buffer;

    xmlEyePath = "../../haarCascade/haarcascade_eye.xml";
    xmlFacePath = "../../haarCascade/haarcascade_frontalface_default.xml";
    xmlMouthPath = "../../haarCascade/haarcascade_mcs_mouth.xml";
    xmlNosePath = "../../haarCascade/haarcascade_mcs_nose.xml";

    //加载
    faceCascade.load(xmlFacePath.toStdString());
    eyeCascade.load(xmlEyePath.toStdString());
    noseCascade.load(xmlNosePath.toStdString());
    mouthCascade.load(xmlMouthPath.toStdString());

    imageRootPath = "../../Images/srcImage";
    imageProducedPath = "../../Images/dstImage/ImageAfterProduce";
    imageFacePath = "../../Images/faceImage";
    imageFaceWithOrgansPath = "../../Images/faceWithOrgansImage";

    isImshowInitialized = false;

    ImgShowWindowLength = 1280;
    ImgShowWindowWidth = 720;

    edgeDetectionType = 1;
    thresholdSegmentationType = 1;
    zoneSegmentationType = 0;

    //获取正则表达
    QRegularExpression reF("^actF_.*");
    QRegularExpression reR("^radBtnR_.*");
    QRegularExpression reE("^radBtnE_.*");
    QRegularExpression reT("^radBtnT_.*");
    QRegularExpression reZ("^radBtnZ_.*");
    actionsF = findChildren<QAction*>(reF);
    radBtnResolution = findChildren<QRadioButton*>(reR);
    radBtnEdgeDetection = findChildren<QRadioButton*>(reE);
    radBtnThresholdSegmentation = findChildren<QRadioButton*>(reT);
    radBtnZoneSegmentation = findChildren<QRadioButton*>(reZ);
    actsF = actionsF;
    radBtnR = radBtnResolution;
    radBtnE = radBtnEdgeDetection;
    radBtnT = radBtnThresholdSegmentation;
    radBtnZ = radBtnZoneSegmentation;

    disableItemsActF(true);

    cameraTimer = new QTimer(this);

    ui->pBtnC_CaptureFrame->setEnabled(false);

    connect(cameraTimer,&QTimer::timeout,this,&MainWindow::updateCameraFrame);
    connect(ui->pBtnC_CameraSwitch,&QPushButton::clicked,this,&MainWindow::updateCameraButton);
    //
    for (QRadioButton* rBR : radBtnR){
        connect(rBR,&QRadioButton::clicked,this,&MainWindow::chooseResolutionRatioType);
    }
    //
    for (QRadioButton* rBE : radBtnE){
        connect(rBE,&QRadioButton::clicked,this,&MainWindow::chooseEdgeDetectionType);
    }
    //
    for (QRadioButton* rBT : radBtnT){
        connect(rBT,&QRadioButton::clicked,this,&MainWindow::chooseThresholdSegmentationType);
    }
    //
    for (QRadioButton* rBZ : radBtnZ){
        connect(rBZ,&QRadioButton::clicked,this,&MainWindow::chooseZoneSegmentationType);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
//禁止使用保存组件（在执行修改操作后允许）
void MainWindow::disableItemsActF(bool isCopy){
    if (isCopy){
        for (QAction* actionf : actsF){
            if (actionf != ui->actF_OpenFile)
                actionf->setEnabled(false);
        }
    }
    else{
        for (QAction* actionf : actionsF){
            if (actionf != ui->actF_OpenFile || actionf != ui->actF_Reload)
                actionf->setEnabled(false);
        }
    }
}

//文档如帮助，欢迎等
//欢迎
void MainWindow::on_actH_Greeting_triggered(){

}

//加载，重载和保存文件
//打开图像
/* What if there's more than one image?
 * Should I add more example to store them?
 * How can I realize the open of multiple images?
 */
void MainWindow::on_actF_OpenFile_triggered(){
    //设置为非帧
    isFrame = false;
    //考虑打开前先清空实例Img中的所有参数？
    QString formatFilter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    //记得去翻一下Qt6书本的第三章槽函数部分，有关于如何绑定一个不需要实例名的槽函数
    QString fileName = QFileDialog::getOpenFileName(this,"选择打开的文件",imageRootPath,formatFilter);
    if (fileName.isEmpty()){
        QMessageBox::critical(this,"加载错误","文件读取失败，文件为空！");
        qDebug() << "Open fault! Empty file!";
        return;
    }
    Img.setFileName(fileName);
    Mat rawImg = imread(fileName.toStdString(),IMREAD_UNCHANGED);
    if (rawImg.empty()){
        QMessageBox::critical(this,"加载错误","图像加载失败，图像为空");
        qDebug() << "Open fault! Empty image";
        return;
    }
    Img.setCVimgOrigin(rawImg);
    Img.setCVimgTemp(rawImg);
    ImgShow(Img.getCVimgOrigin());
    ui->statusbar->showMessage(Img.getFileName());
    enableItems();
}

//通过原图像路径覆盖式保存更改后的图像
//若是从摄像头视频帧中截取的，没有原路径的图像该如何解决？
void MainWindow::on_actF_SaveFile_triggered(){
    if (Img.getQimgTemp().isNull()){
        qDebug() << "Save fault! Empty m_Qimg_Temp!";
        return;
    }
    QImage tobeSavedImg;
    QString name;
    if (isFrame){
        tobeSavedImg = Frame.getQimgTemp().copy();
        //
        name = Img.getFileName();
    }
    else {
        tobeSavedImg = Img.getQimgTemp().copy();
        name = Img.getFileName();
    }
    tobeSavedImg.save(name);
    disableItemsActF(false);
}

//另存为图像为另一个独立的路径
void MainWindow::on_actF_SaveFileAsCopy_triggered(){
    QImage tobeSavedImg;
    if (!capture.isOpened() && !isFrame) {
        if (Img.getQimgTemp().isNull()){
            qDebug() << "Save copy fault! Empty m_Qimg_Temp!";
            return;
        }
        tobeSavedImg = Img.getQimgTemp().copy();
    }
    else {
        if (Frame.getQimgTemp().isNull()){
            qDebug() << "Save copy fault! Empty m_Qimg_Temp!";
            return;
        }
        tobeSavedImg = Frame.getQimgTemp().copy();
    }
    QString formatFilter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                           "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    //imageRootPath似乎没有将文件夹正确地导向对应的路径？
    QString fileName = QFileDialog::getSaveFileName(this,"保存文件",imageRootPath,formatFilter);
    tobeSavedImg.save(fileName);
    disableItemsActF(true);
}

//重新加载图片
void MainWindow::on_actF_Reload_triggered(){
    if (Img.getQimgOrigin().isNull()){
        qDebug() << "Reload fault! Empty m_Qimg_Origin!";
    }
    QImage tobeShowedImg = Img.getQimgOrigin().copy();
    QPixmap pixmap = QPixmap::fromImage(tobeShowedImg.scaled(ui->showImg->size(),Qt::KeepAspectRatio));
    ui->showImg->setPixmap(pixmap);
    Img.resetElements();
    disableItemsActF(true);
}

//以上功能的实现所需函数
void MainWindow::ImgShow(Mat src){
    QImage image;
    if (!isFrame){
        Img.cvtCV2Qimg(src.clone());
        image = Img.getQimgTemp();
        QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->showImg->size(),Qt::KeepAspectRatio));
        ui->showImg->setPixmap(pixmap);
    }
    else {
        Frame.cvtCV2Qimg(src.clone());
        image = Frame.getQimgTemp();
        QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->showImg->size(),Qt::KeepAspectRatio));
        ui->showImg->setPixmap(pixmap);
    }
}

//启用组件
void MainWindow::enableItems(){
    if (!capture.isOpened()){
        for (QAction* actf : actsF){
            actf->setEnabled(true);
        }
    }
    else{
        for (QAction* actf : actsF){
            if (actf != ui->actF_SaveFile){
                actf->setEnabled(true);
            }
        }
    }
}

//摄像头相关操作
//更新打开/关闭窗口文本
void MainWindow::updateCameraButton(){
    if (capture.isOpened()){
        ui->pBtnC_CameraSwitch->setText("关闭摄像头");
        ui->pBtnC_CaptureFrame->setEnabled(true);
    }
    else {
        ui->pBtnC_CameraSwitch->setText("打开摄像头");
        ui->pBtnC_CaptureFrame->setEnabled(false);
    }
}

//更新帧、设置摄像头窗口大小
void MainWindow::updateCameraFrame(){
    Mat frame;
    capture.read(frame);
    Frame.setCVimgTemp(frame.clone());
    if (!isImshowInitialized){
        namedWindow("摄像头0",WINDOW_NORMAL);
        isImshowInitialized = true;
    }
    //此处需要异常处理，不然直接关闭摄像头窗口会报错并强制终止程序
    resizeWindow("摄像头0",ImgShowWindowLength,ImgShowWindowWidth);
    imshow("摄像头0",Frame.getCVimgTemp());
}

//展示倒计时弹窗
void MainWindow::showCameraPop(){
    countDown = 5;
    msgBoxInfo001 = new QMessageBox(this);
    msgBoxInfo001->setWindowTitle("提示！");
    this->msgBoxInfo001->setText(QString("摄像头0将在%1秒后打开").arg(countDown));
    msgBoxInfo001->setStandardButtons(QMessageBox::NoButton);
    msgBoxInfo001->setModal(false);
    this->msgBoxInfo001->show();
    countDownTimer = new QTimer(this);
    connect(countDownTimer,&QTimer::timeout,this,&MainWindow::updateCountDown);
    countDownTimer->start(1000);
}

//更新倒数
void MainWindow::updateCountDown(){
    countDown--;
    if (countDown>0){
        msgBoxInfo001->setText(QString("摄像头0将在%1秒后打开").arg(countDown));
    }
    else {
        countDownTimer->stop();
        msgBoxInfo001->close();
        msgBoxInfo001->deleteLater();
    }
}

//控制摄像头开/关
void MainWindow::on_pBtnC_CameraSwitch_clicked(){
    if (capture.isOpened()){
        cameraTimer->stop();
        capture.release();
        destroyWindow("摄像头0");      //关闭摄像头窗口
    } else {
        isFrame = true;
        capture.open(0);
        if (!capture.isOpened()){
            QMessageBox::critical(this,"打开错误！","摄像头打开失败");
            return;
        }
        ui->pBtnC_CameraSwitch->setEnabled(false);
        showCameraPop();
        waitKey(5000);
        ui->pBtnC_CameraSwitch->setEnabled(true);
        cameraTimer->start(16);
    }
}

//在Qt界面上显示捕获的帧
void MainWindow::on_pBtnC_CaptureFrame_clicked(){
    if (!isFrame){
        ImgShow(Img.getCVimgTemp());
    }
    else{
        ImgShow(Frame.getCVimgTemp());
    }
    enableItems();
}

//
void MainWindow::on_pBtnC_ConductDetection_clicked(){
    produceImg();
}

//选择摄像头窗口分辨率
void MainWindow::chooseResolutionRatioType(){
    if (ui->radBtnR_ResolutionRatio1280x720->isChecked()){
        ImgShowWindowLength = 1280;
        ImgShowWindowWidth = 720;
    }
    else if (ui->radBtnR_ResolutionRatio1440x810->isChecked()){
        ImgShowWindowLength = 1440;
        ImgShowWindowWidth = 810;
    }
    else if (ui->radBtnR_ResolutionRatio1600x900->isChecked()){
        ImgShowWindowLength = 1600;
        ImgShowWindowWidth = 900;
    }
    else if (ui->radBtnR_ResolutionRatio1760x990->isChecked()){
        ImgShowWindowLength = 1760;
        ImgShowWindowWidth = 990;
    }
    else if (ui->radBtnR_ResolutionRatio1920x1080->isChecked()){
        ImgShowWindowLength = 1920;
        ImgShowWindowWidth = 1080;
    }
    qDebug() << ImgShowWindowLength << "x" << ImgShowWindowWidth;
}

//选择边缘检测算子
void MainWindow::chooseEdgeDetectionType(){
    if (ui->radBtnE_edgeDetection_Roberts->isChecked()){
        edgeDetectionType = 0;
    }
    else if (ui->radBtnE_edgeDetection_Prewitt->isChecked()){
        edgeDetectionType = 2;
    }
    else if (ui->radBtnE_edgeDetection_Sobel->isChecked()){
        edgeDetectionType = 1;
    }
    else if (ui->radBtnE_edgeDetection_Laplacian->isChecked()){
        edgeDetectionType = 3;
    }
    qDebug() << edgeDetectionType;
}

//选择阈值分割算法
void MainWindow::chooseThresholdSegmentationType(){
    if (ui->radBtnI_ImageSplit_AdaptiveThreshold->isChecked()){
        thresholdSegmentationType = 1;
    }
    else if (ui->radBtnI_ImageSplit_DoubleHill->isChecked()){
        thresholdSegmentationType = 0;
    }
    else if (ui->radBtnI_ImageSplit_Iteration->isChecked()){
        thresholdSegmentationType = 2;
    }
    qDebug() << thresholdSegmentationType;
}

//区域分割算法
void MainWindow::chooseZoneSegmentationType(){
    if (ui->radBtnZ_ZoneSegmentation_Skin->isChecked()){
        zoneSegmentationType = 0;
    }
    qDebug() << zoneSegmentationType;
}

/*  图像预处理,或许该放进image头文件中？那么要修改QMessageBox弹窗逻辑
    在ImgShow时调用？
    可以添加直方图的绘制
    基于参考白的光线补偿*/
Mat MainWindow::lightCompensation(Mat src){
    int width,height;
    Mat dst = Mat::zeros(src.size(),src.type());
    width = src.cols;
    height = src.rows;
    int pixelCount = 0;
    int grayNum = 255;
    int grayAverage,graySum;
    float lightCoe;
    int pixelNum = width*height;

    int histSize = 256;
    float range[] = {0,256};
    const float* histRange = {range};
    Mat hist;
    calcHist(&src,1,0,Mat(),hist,1,&histSize,&histRange);

    const float thresholdcoe = 0.05;

    //找到灰度阈值
    for (int i=255;i>=0;--i){
        pixelCount += static_cast<int>(hist.at<float>(i));
        if ((float)pixelCount / pixelNum >= thresholdcoe){
            grayNum = i;
            break;
        }
    }

    //计算高灰度区域灰度平均值
    pixelCount = 0;
    graySum = 0;
    for (int i=grayNum;i<256;i++){
        int gray = static_cast<int>(hist.at<float>(i));
        graySum += gray*i;
        pixelCount += gray;
    }
    grayAverage = graySum/pixelCount;

    lightCoe = 255.0f / grayAverage;

    src.convertTo(dst,-1,lightCoe,0);

    return dst;
}

//边缘检测
//也许可以添加一个Canny算法
Mat MainWindow::edgeDetection(){
    Mat src,dst;
    if (isFrame){
        src = Frame.getCVimgTemp();
    }
    else {
        src = Img.getCVimgTemp();
    }

    if (edgeDetectionType==0){
        Mat RobertsX,RobertsY;
        //设计卷积核
        Mat kernelX = (Mat_<float>(2,2) << 0, 1, -1, 0);
        Mat kernelY = (Mat_<float>(2,2) << 1, 0, 0, -1);
        //卷积操作
        //此处设计8U，可能出现问题？
        filter2D(src,RobertsX,CV_8U,kernelX);
        filter2D(src,RobertsY,CV_8U,kernelY);
        //对矩阵各个值求绝对值
        convertScaleAbs(RobertsX,RobertsX);
        convertScaleAbs(RobertsY,RobertsY);
        //合并
        addWeighted(RobertsX,0.5,RobertsY,0.5,0,dst);
    }
    else if (edgeDetectionType==1){
        Mat SobelX,SobelY;
        //设计卷积核
        Mat kernelX = (Mat_<float>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
        Mat kernelY = (Mat_<float>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        //卷积操作
        //此处设计8U，可能出现问题？
        filter2D(src,SobelX,CV_8U,kernelX);
        filter2D(src,SobelY,CV_8U,kernelY);
        //对矩阵各个值求绝对值
        convertScaleAbs(SobelX,SobelX);
        convertScaleAbs(SobelY,SobelY);
        //合并
        addWeighted(SobelX,0.5,SobelY,0.5,0,dst);
    }
    else if (edgeDetectionType==2){
        Mat PrewittX,PrewittY;
        //设计卷积核
        Mat kernelX = (Mat_<float>(3,3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
        Mat kernelY = (Mat_<float>(3,3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
        //卷积操作
        //此处设计8U，可能出现问题？
        filter2D(src,PrewittX,CV_8U,kernelX);
        filter2D(src,PrewittY,CV_8U,kernelY);
        //对矩阵各个值求绝对值
        convertScaleAbs(PrewittX,PrewittX);
        convertScaleAbs(PrewittY,PrewittY);
        //合并
        addWeighted(PrewittX,0.5,PrewittY,0.5,0,dst);
    }
    else if (edgeDetectionType==3){
        Mat laplacian;
        //拉普拉斯二阶导
        Laplacian(src,dst,CV_8U,3);
        //对矩阵各个值求绝对值
        convertScaleAbs(dst,dst);
    }

    return dst;
}

//阈值分割
Mat MainWindow::thresholdSegmentation(){
    Mat src,dst;
    if (isFrame){
        src = Frame.getCVimgTemp();
    } else {
        src = Img.getCVimgTemp();
    }

    //直方图双峰
    if (thresholdSegmentationType==0){
        int length = src.cols;
        int width = src.rows;
        int gray;
        int prev,now,next;
        int threshold_001,threshold_002;
        Mat Gaussianed;
        vector<int> grayList(256,0);
        vector<int> hillTop(0);

        //先高斯滤波平缓一下像素直方图
        GaussianBlur(src,Gaussianed,Size(3,3),0,0);

        for (int i=0;i<length;i++){
            for (int j=0;j<width;j++){
                gray = src.at<uchar>(i,j);
                grayList[gray]++;
            }
        }

        for (int i=1;i<255;i++){
            prev = grayList[i-1];
            now = grayList[i];
            next = grayList[i+1];

            if ((prev<now)&&(next<now)){
                hillTop.push_back(i);
            }
        }

        if (hillTop.size() < 2){
            QMessageBox::critical(this,"直方图双峰法实现错误！","该图像直方图只存在一个峰,本次流程未执行阈值分割!");
            return src;
        }
        //最后一个代表降序,less<type>()代表升序
        sort(hillTop.begin(),hillTop.end(),greater<int>());

        threshold_001 = hillTop[0];
        threshold_002 = hillTop[1];

        for (int i=0;i<length;i++){
            for (int j=0;j<width;j++){
                if (src.at<uchar>(i,j) > threshold_001)
                    dst.at<uchar>(i,j) = 255;
                else if ((src.at<uchar>(i,j) < threshold_001) && (threshold_002 < src.at<uchar>(i,j)))
                    dst.at<uchar>(i,j) = 128;
                else if (src.at<uchar>(i,j) < threshold_002)
                    dst.at<uchar>(i,j) = 0;
            }
        }
    }
    //自适应阈值
    else if (thresholdSegmentationType==1){
        adaptiveThreshold(src,dst,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,10);
    }
    //迭代阈值
    else if (thresholdSegmentationType==2){
        src.convertTo(src,CV_32F);
        double zmax,zmin;
        minMaxLoc(src,&zmin,&zmax,0,0);
        int tk = (zmax+zmin) / 2;

        bool b = true;
        int length = src.cols;
        int width = src.rows;

        while (b){
            double ifg = 0;
            double ibg = 0;
            double fnum = 0;
            double bnum = 0;

            for (int i=0;i<length;i++){
                for (int j=0;j<width;j++){
                    float pixel = src.at<float>(i,j);
                    if (pixel >= tk){
                        ifg += pixel;
                        fnum++;
                    } else {
                        ibg += pixel;
                        bnum++;
                    }
                }
            }

            if (ifg==0 || ibg==0)   break;

            int zb = ibg / bnum;
            int zf = ifg / fnum;

            int new_tk = (zb + zf) / 2;
            if (tk == new_tk){
                b = false;
            } else {
                tk = new_tk;
            }
        }

        threshold(src,dst,tk,255,THRESH_BINARY);
    }


    return dst;
}

//区域分割
Mat MainWindow::zoneSegmentation(){
    Mat src,hsv,dst;
    if (isFrame){
        src = Frame.getCVimgTemp();
    } else {
        src = Img.getCVimgTemp();
    }

    //肤色分割
    if (zoneSegmentationType==0){
        cvtColor(src,hsv,COLOR_BGR2HSV);
        //设定肤色范围
        //inRange(src,bottom,top,dst)
        inRange(hsv,Scalar(0,48,80),Scalar(20,255,255),dst);
        /* erode 腐蚀函数
         * void erode(InputArray src,
         *          OutputArray dst,
         *          InputArray kernel,
         *          Point anchor=Point(-1, -1),
         *          int iterations=1,
         *          int borderType=BORDER_CONSTANT,
         *          const Scalar& borderValue=morphologyDefaultBorderValue());
         *  第一个参数：InputArray类型的src，输入图像，Mat类的对象即可。图像的通道数可以是任意的，但是图像的深度应该是CV_8U，CV_16U，CV_16S，CV_32F，CV_64F之一。
         *  第二个参数：OutputArray类型的dst，目标图像，需要和输入图像有一样的尺寸和类型。
         *  第三个参数：InputArray类型的kernel，腐蚀操作的核。当为NULL时，表示的是使用的是参考点位于中心3*3的核。
         *  第四个参数：Point类型的anchor，锚点的位置，默认值是 (-1, -1)，表示位于中心。
         *  第五个参数：int类型的iterations，迭代的次数，默认值是1。
         *  第六个参数：int类型的borderType，用于推断图像外部像素的某种边界模式，默认值是BORDER_DEFAULT。
         *  第七个参数：const Scalar&类型的borderValue，一般不管它
         */
        erode(dst,dst,Mat(),Point(-1,-1),2);
        //膨胀函数，参数与erode一样
        dilate(dst,dst,Mat(),Point(-1,-1),2);
    }

    return dst;
}


//对图像执行预处理、边缘提取、图像分割、区域标记、轮廓跟踪和特征提取
void MainWindow::produceImg(){
    Mat src = isFrame ? Frame.getCVimgTemp() : Img.getCVimgTemp();
    //图像预处理
    Mat afterTrans = Mat::zeros(src.size(),src.type());
    cvtColor(src,afterTrans,COLOR_BGR2GRAY);
    qDebug() << "灰度转化完成";
    Mat afterComp = lightCompensation(afterTrans);
    qDebug() << "光线平衡完成";
    Mat afterHist = Mat::zeros(src.size(),src.type());
    equalizeHist(afterComp,afterHist);
    qDebug() << "直方图均衡化完成";

    if (isFrame){
        Frame.setCVimgTemp(afterHist);
        Frame.saveWithCount(Frame.getCVimgTemp(),imageProducedPath,PRE_PRODUCE);
    } else {
        Img.setCVimgTemp(afterHist);
        Img.saveWithCount(Img.getCVimgTemp(),imageProducedPath,PRE_PRODUCE);
    }
    //区域分割
    Mat afZSegmentation = zoneSegmentation();
    if (isFrame){
        Frame.saveWithCount(afZSegmentation,imageProducedPath,ZONE_MARK);
    } else {
        Img.saveWithCount(afZSegmentation,imageProducedPath,ZONE_MARK);
    }
    //人脸识别
    //1.识别人脸
    vector<Rect> faces;
    Mat image;
    if (isFrame){
        image = Frame.getCVimgTemp();
    } else {
        image = Img.getCVimgTemp();
    }
    /*  void CascadeClassifier::detectMultiScale(
     *      InputArray image,                  // 输入图像（灰度图）
     *      std::vector<Rect>& objects,        // 输出检测到的目标矩形
     *      double scaleFactor = 1.1,          // 金字塔图像缩放比例因子   值up -> 减少缩放次数，以时间换精度
     *      int minNeighbors = 3,              // 最小邻近数（过滤误检）
     *      int flags = 0,                     // 旧版本参数（已废弃，通常设为0）
     *      Size minSize = Size(),             // 目标最小尺寸（过滤过小目标）
     *      Size maxSize = Size()              // 目标最大尺寸（过滤过大目标）
     *  );
     */
    faceCascade.detectMultiScale(image,faces,1.1,3);
    if (isFrame){
        Frame.saveWithCount(image,imageFacePath,FACE_DETECT);
    } else {
        Img.saveWithCount(image,imageFacePath,FACE_DETECT);
    }
    for (const Rect& face : faces){
        //绘制人脸框
        rectangle(image,face,Scalar(0,255,0),2);
        //人脸区域内检测五官
        Mat faceROI = image(face);

        //2.识别眼睛
        vector<Rect> eyes;
        eyeCascade.detectMultiScale(faceROI, eyes, 1.1, 2);
        for (const Rect& eye : eyes){
            //tl() -> Top-Left -> 返回矩阵 的 左上角顶点 在 原图像中的坐标
            //br() -> Bottom-Right -> 返回矩阵 的 右下角顶点 在 原图像中的坐标
            rectangle(faceROI, face.tl()+eye.tl(), face.tl()+eye.br(), Scalar(255,0,0), 2);
        }

        //3.识别鼻子
        vector<Rect> noses;
        noseCascade.detectMultiScale(faceROI, noses, 1.1, 2);
        for (const Rect& nose : noses){
            rectangle(faceROI, face.tl()+nose.tl(), face.tl()+nose.br(), Scalar(0,0,255), 2);
        }

        //4.识别嘴巴
        vector<Rect> mouths;
        mouthCascade.detectMultiScale(faceROI, mouths, 1.1, 2);
        for (const Rect& mouth : mouths){
            rectangle(faceROI, face.tl()+mouth.tl(), face.tl()+mouth.br(), Scalar(255,0,255), 2);
        }

        if (isFrame){
            Frame.saveWithCount(faceROI,imageFaceWithOrgansPath,FACE_DETECT);
        } else {
            Img.saveWithCount(faceROI,imageFaceWithOrgansPath,FACE_DETECT);
        }
    }
    //阈值分割
    Mat afTSegmentation = thresholdSegmentation();
    if (isFrame){
        Frame.saveWithCount(afTSegmentation,imageProducedPath,THRE_SEG);
    } else {
        Img.saveWithCount(afTSegmentation,imageProducedPath,THRE_SEG);
    }
    //边缘提取
    Mat afDetection = edgeDetection();
    if (isFrame){
        Frame.saveWithCount(afDetection,imageProducedPath,EDGE_DETEC);
    } else {
        Img.saveWithCount(afDetection,imageProducedPath,EDGE_DETEC);
    }
}

