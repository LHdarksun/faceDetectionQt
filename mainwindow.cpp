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
    imageFaceROIPath = "../../Images/faceROI";
    imageMaskPath = "../../Images/faceMask";

    isImshowInitialized = false;
    isMachineLearning = false;

    ImgShowWindowLength = 1280;
    ImgShowWindowWidth = 720;

    edgeDetectionType = 1;
    thresholdSegmentationType = 1;
    traditionalFaceType = 0;

    //获取正则表达
    QRegularExpression reF("^actF_.*");
    QRegularExpression reR("^radBtnR_.*");
    QRegularExpression reE("^radBtnE_.*");
    QRegularExpression reT("^radBtnT_.*");
    actionsF = findChildren<QAction*>(reF);
    radBtnResolution = findChildren<QRadioButton*>(reR);
    radBtnEdgeDetection = findChildren<QRadioButton*>(reE);
    radBtnThresholdSegmentation = findChildren<QRadioButton*>(reT);
    actsF = actionsF;
    radBtnR = radBtnResolution;
    radBtnE = radBtnEdgeDetection;
    radBtnT = radBtnThresholdSegmentation;

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
    chooseFaceDetectionType();
    ui->chooseFuncWidget->setCurrentWidget(ui->funcProduce);
    QMessageBox::information(this,"成功","人脸ROI保存成功，请在当前界面选择人脸ROI并处理");
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

void MainWindow::updateFaceDetectionType(){
    if (isMachineLearning){
        isMachineLearning = false;
        ui->pBtnC_MethodSwitch->setText("转化为机器学习方法");
        qDebug() << "现在是机器学习";
    } else {
        isMachineLearning = true;
        ui->pBtnC_MethodSwitch->setText("转化为传统方法");
        qDebug() << "现在是传统方法";
    }
}

void MainWindow::chooseFaceDetectionType(){
    Mat src = isFrame ? Frame.getCVimgTemp() : Img.getCVimgTemp();
    if (isMachineLearning){
        faceDetectionMachineLearning(src);
        qDebug() << "执行机器学习法";
    } else {
        auto *dlg = new chooseTraditionalDetectionTypeWidget(this);
        connect(dlg,&chooseTraditionalDetectionTypeWidget::typeSelected,this,[=](int type){
            traditionalFaceType = type;
            qDebug() << "传统检测方法：" << type;
        });
        dlg->exec();
        faceDetectionTraditional(src);
        qDebug() << "执行传统算法";
    }
}

void MainWindow::on_pBtnC_MethodSwitch_clicked(){
    updateFaceDetectionType();
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
Mat MainWindow::edgeDetection(Mat src){
    Mat dst;

    switch (edgeDetectionType){
        case 0: {
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
        case 1: {
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
        case 2: {
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
        case 3: {
            Mat laplacian;
            //拉普拉斯二阶导
            Laplacian(src,dst,CV_8U,3);
            //对矩阵各个值求绝对值
            convertScaleAbs(dst,dst);
        }
    }

    return dst;
}

//阈值分割
Mat MainWindow::thresholdSegmentation(Mat src){
    Mat dst;

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
void MainWindow::faceDetectionTraditional(Mat src){
    Mat gray,hsv,mask,dst;
    src = isFrame ? Frame.getCVimgTemp() : Img.getCVimgTemp();
    vector<vector<Point>> contours;
    if (traditionalFaceType==0) {
        cvtColor(src.clone(),hsv,COLOR_BGR2HSV);
        inRange(hsv,Scalar(0,48,80),Scalar(20,255,255),mask);

        //腐蚀与膨胀
        erode(mask,mask,Mat(),Point(-1,-1),2);
        dilate(mask,mask,Mat(),Point(-1,-1),2);

        GaussianBlur(src.clone(),dst,Size(3,3),0);
        qDebug() << "高斯滤波完成";
        isFrame ? Frame.saveWithCount(dst,imageProducedPath,FACE_DETECT) :
        Img.saveWithCount(dst,imageProducedPath,FACE_DETECT);

        findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        if (!contours.empty()){
            //找到最大轮廓
            int maxIdx = 0;
            double maxArea = contourArea(contours[0]);
            for (size_t i=1;i<contours.size();++i){
                double area = contourArea(contours[i]);
                if (area > maxArea){
                    maxArea = area;
                    maxIdx = i;
                }
            }
            //取最大轮廓的包围盒中心
            Rect bound = boundingRect(contours[maxIdx]);
            Point seedPoint = Point(bound.x + bound.width/2, bound.y + bound.height/2);

            Mat regionGrown = Mat::zeros(mask.size(),CV_8UC1);
            Scalar newVal = Scalar(255);
            int loDiff = 10, upDiff = 10;
            int flags = 4 | FLOODFILL_MASK_ONLY | (255 << 8);

            Mat maskForFlood = Mat::zeros(mask.rows + 2, mask.cols + 2, CV_8UC1);

            floodFill(mask,maskForFlood,seedPoint,newVal,0,Scalar(loDiff),Scalar(upDiff),flags);

            regionGrown = maskForFlood(Range(1,mask.rows+1),Range(1,mask.cols+1));

            mask = regionGrown.clone();

            isFrame ? Frame.saveWithCount(mask,imageMaskPath,MASK_GET) :
                Img.saveWithCount(mask,imageMaskPath,MASK_GET);
            qDebug() << "mask图保存成功";
        }
    } else if (traditionalFaceType==1) {
        cvtColor(src,gray,COLOR_BGR2GRAY);

        gray = lightCompensation(gray);
        qDebug() << "光线平衡完成";

        Mat threshImg = thresholdSegmentation(gray);
        qDebug() << "阈值分割完成";

        Mat edgeImg = edgeDetection(threshImg);
        qDebug() << "边缘检测完成";

        isFrame ? Frame.saveWithCount(edgeImg,imageProducedPath,PRE_PRODUCE) :
            Img.saveWithCount(edgeImg,imageProducedPath,PRE_PRODUCE);

        findContours(edgeImg,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    }
    for (const auto& contour : contours){
        //用找到的边缘去形成矩阵
        Rect bound = boundingRect(contour);
        //人脸宽高比,一般为0.75~1.3
        float aspectRatio = (float)bound.width / bound.height;
        if (bound.area() > 900 && aspectRatio > 0.75 && aspectRatio < 1.3){
            rectangle(src,bound,Scalar(0,255,0),2);
        }

        Mat faceROI = src(bound).clone();
        isFrame ? Frame.saveWithCount(faceROI,imageFaceROIPath,FACE_DETECT) :
            Img.saveWithCount(faceROI,imageFaceROIPath,FACE_DETECT);
    }
    isFrame ? Frame.saveWithCount(src,imageFacePath,FACE_DETECT) :
        Img.saveWithCount(src,imageFacePath,FACE_DETECT);
}

void MainWindow::faceDetectionMachineLearning(Mat src){
    vector<Rect> faces;
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
    faceCascade.detectMultiScale(src,faces,1.1,3);
    for (const Rect& face : faces){
        //绘制人脸框
        rectangle(src,face,Scalar(0,255,0),2);
        //将人脸框部分存储为一个Mat类型图像
        Mat faceROI = src(face).clone();
        isFrame ? Frame.saveWithCount(faceROI,imageFaceROIPath,FACE_DETECT) :
                  Img.saveWithCount(faceROI,imageFaceROIPath,FACE_DETECT);
    }
    isFrame ? Frame.saveWithCount(src,imageFacePath,FACE_DETECT) :
              Img.saveWithCount(src,imageFacePath,FACE_DETECT);
}

void MainWindow::on_pBtn_ChooseROI_clicked(){
    QString formatFilter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                           "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    QString fileName = QFileDialog::getOpenFileName(this,"选择打开的文件",imageFaceROIPath,formatFilter);
    if (fileName.isEmpty()){
        QMessageBox::critical(this,"加载错误","文件读取失败，文件为空！");
        qDebug() << "Open fault! Empty file!";
        return;
    }
    faceDetect.setFileName(fileName);
    Mat rawImg = imread(fileName.toStdString(),IMREAD_UNCHANGED);
    if (rawImg.empty()){
        QMessageBox::critical(this,"加载错误","图像加载失败，图像为空");
        qDebug() << "Open fault! Empty image";
        return;
    }
    faceDetect.setCVimgOrigin(rawImg);
    ImgShow(faceDetect.getCVimgOrigin());
    ui->statusbar->showMessage(faceDetect.getFileName());
}

void MainWindow::on_pBtn_FindOrgans_clicked(){
    Mat faceROI = faceDetect.getCVimgOrigin();
    vector<Rect> eyes;
    eyeCascade.detectMultiScale(faceROI, eyes, 1.1, 2);
    for (const Rect& eye : eyes){
        //tl() -> Top-Left -> 返回矩阵 的 左上角顶点 在 原图像中的坐标
        //br() -> Bottom-Right -> 返回矩阵 的 右下角顶点 在 原图像中的坐标
        rectangle(faceROI, eye.tl(), eye.br(), Scalar(255,0,0), 2);
    }

    //3.识别鼻子
    vector<Rect> noses;
    noseCascade.detectMultiScale(faceROI, noses, 1.1, 2);
    for (const Rect& nose : noses){
        rectangle(faceROI, nose.tl(), nose.br(), Scalar(0,0,255), 2);
    }

    //4.识别嘴巴
    vector<Rect> mouths;
    mouthCascade.detectMultiScale(faceROI, mouths, 1.1, 2);
    for (const Rect& mouth : mouths){
        rectangle(faceROI, mouth.tl(), mouth.br(), Scalar(255,0,255), 2);
    }

    if (isFrame){
        Frame.saveWithCount(faceROI,imageFaceWithOrgansPath,FACE_DETECT);
    } else {
        Img.saveWithCount(faceROI,imageFaceWithOrgansPath,FACE_DETECT);
    }

    faceDetect.cvtCV2Qimg(faceROI);
    faceDetect.setCVimgTemp(faceROI);

    QLabel* label = new QLabel;
    label->setPixmap(QPixmap::fromImage(faceDetect.getQimgTemp()));
    label->setWindowTitle("显示五官");
    label->resize(faceDetect.getQimgTemp().width(),faceDetect.getQimgTemp().height());
    label->show();
}

Mat MainWindow::colorQuantization(Mat src, int k) {
    Mat data;
    src.convertTo(data, CV_32F);
    data = data.reshape(1, data.total());
    Mat labels, centers;
    kmeans(data, k, labels,
           TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10, 1.0),
           3, KMEANS_PP_CENTERS, centers);
    centers = centers.reshape(3, centers.rows);
    Mat newImg(data.size(), data.type());
    for (int i = 0; i < data.rows; i++) {
        newImg.at<float>(i, 0) = centers.at<float>(labels.at<int>(i), 0);
        newImg.at<float>(i, 1) = centers.at<float>(labels.at<int>(i), 1);
        newImg.at<float>(i, 2) = centers.at<float>(labels.at<int>(i), 2);
    }
    newImg = newImg.reshape(3, src.rows);
    newImg.convertTo(newImg, CV_8U);
    return newImg;
}

// 简单漫画风格化
Mat MainWindow::Cartoonify(Mat src) {
    Mat img, gray, edges, color, cartoon;
    // 1. 双边滤波，保边去噪
    bilateralFilter(src, img, 9, 75, 75);
    // 2. 灰度并高斯模糊
    cvtColor(img, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 7);
    // 3. 边缘检测（自适应阈值）
    adaptiveThreshold(gray, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);
    // 4. 色彩量化
    color = colorQuantization(img, 8);
    // 5. 合成漫画效果（与边缘融合）
    cvtColor(edges, edges, COLOR_GRAY2BGR);
    cartoon = color & edges;
    return cartoon;
}

void MainWindow::on_pBtn_CartoonType_clicked(){
    Mat src = faceDetect.getCVimgOrigin();
    Mat cartoon = Cartoonify(src);

    faceDetect.cvtCV2Qimg(cartoon);
    faceDetect.setCVimgTemp(cartoon);

    faceDetect.saveWithCount(cartoon,imageFaceWithOrgansPath,FACE_DETECT);

    QLabel* label = new QLabel;
    label->setPixmap(QPixmap::fromImage(faceDetect.getQimgTemp()));
    label->setWindowTitle("漫画化");
    label->resize(faceDetect.getQimgTemp().width(),faceDetect.getQimgTemp().height());
    label->show();
}

void MainWindow::on_pBtn_FunhouseType_clicked(){
    Mat src = faceDetect.getCVimgOrigin();
    Mat dst = Mat::zeros(src.size(), src.type());

    int rows = src.rows;
    int cols = src.cols;

    // 构建映射表
    Mat map_x = Mat::zeros(src.size(), CV_32FC1);
    Mat map_y = Mat::zeros(src.size(), CV_32FC1);

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            // 示例1：水平方向正弦形变
            float offset_x = 30.0f * sin(2 * CV_PI * y / 180);
            float new_x = x + offset_x;

            // 示例2：垂直方向镜面变形（可选）
            // float offset_y = 30.0f * cos(2 * CV_PI * x / 180);
            // float new_y = y + offset_y;

            // 确保坐标在有效范围内
            map_x.at<float>(y, x) = std::min(std::max(new_x, 0.0f), static_cast<float>(cols - 1));
            map_y.at<float>(y, x) = static_cast<float>(y);
        }
    }

    remap(src, dst, map_x, map_y, INTER_LINEAR, BORDER_REPLICATE);

    faceDetect.cvtCV2Qimg(dst);
    faceDetect.setCVimgTemp(dst);

    faceDetect.saveWithCount(dst,imageFaceWithOrgansPath,FACE_DETECT);

    QLabel* label = new QLabel;
    label->setPixmap(QPixmap::fromImage(faceDetect.getQimgTemp()));
    label->setWindowTitle("哈哈镜");
    label->resize(faceDetect.getQimgTemp().width(),faceDetect.getQimgTemp().height());
    label->show();
}

Mat MainWindow::AddPatternBorder(Mat src, Mat pattern, int borderWidth) {
    int rows = src.rows + borderWidth * 2;
    int cols = src.cols + borderWidth * 2;

    // 1. 创建比原图更大的画布
    Mat dst(rows, cols, src.type());

    // 2. 复制原图到画布中心
    src.copyTo(dst(Rect(borderWidth, borderWidth, src.cols, src.rows)));

    // 3. 调整花纹图片尺寸
    Mat patternResize;
    cv::resize(pattern, patternResize, Size(borderWidth, borderWidth));

    // 4. 贴四角
    patternResize.copyTo(dst(Rect(0, 0, borderWidth, borderWidth)));                                 // 左上
    patternResize.copyTo(dst(Rect(cols - borderWidth, 0, borderWidth, borderWidth)));               // 右上
    patternResize.copyTo(dst(Rect(0, rows - borderWidth, borderWidth, borderWidth)));               // 左下
    patternResize.copyTo(dst(Rect(cols - borderWidth, rows - borderWidth, borderWidth, borderWidth))); // 右下

    // 5. 贴四边
    for (int x = borderWidth; x < cols - borderWidth; x += borderWidth) {
        patternResize.copyTo(dst(Rect(x, 0, borderWidth, borderWidth))); // 顶部
        patternResize.copyTo(dst(Rect(x, rows - borderWidth, borderWidth, borderWidth))); // 底部
    }
    for (int y = borderWidth; y < rows - borderWidth; y += borderWidth) {
        patternResize.copyTo(dst(Rect(0, y, borderWidth, borderWidth))); // 左侧
        patternResize.copyTo(dst(Rect(cols - borderWidth, y, borderWidth, borderWidth))); // 右侧
    }

    return dst;
}

Mat MainWindow::GenerateCheckerPattern(int size, int blockSize, Scalar color1, Scalar color2) {
    Mat pattern(size, size, CV_8UC3, color1);
    for (int y = 0; y < size; y += blockSize) {
        for (int x = 0; x < size; x += blockSize) {
            if (((x / blockSize) + (y / blockSize)) % 2 == 0)
                rectangle(pattern, Rect(x, y, blockSize, blockSize), color2, FILLED);
        }
    }
    return pattern;
}

void MainWindow::on_pBtn_AddEdge_clicked(){
    Mat src = faceDetect.getCVimgOrigin();

    Mat checkerPattern = GenerateCheckerPattern(40,9,Scalar(0),Scalar(255));
    Mat withBorder = AddPatternBorder(src,checkerPattern,40);

    faceDetect.cvtCV2Qimg(withBorder);
    faceDetect.setCVimgTemp(withBorder);

    faceDetect.saveWithCount(withBorder,imageFaceWithOrgansPath,FACE_DETECT);

    QLabel* label = new QLabel;
    label->setPixmap(QPixmap::fromImage(faceDetect.getQimgTemp()));
    label->setWindowTitle("棋盘边框");
    label->resize(faceDetect.getQimgTemp().width(),faceDetect.getQimgTemp().height());
    label->show();
}
