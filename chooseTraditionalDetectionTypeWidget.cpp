#include "chooseTraditionalDetectionTypeWidget.h"

chooseTraditionalDetectionTypeWidget::chooseTraditionalDetectionTypeWidget(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("选择传统检测方法");
    pBtnSkin = new QPushButton("肤色分割",this);
    pBtnThreshEdge = new QPushButton("阈值+边缘分割",this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(pBtnSkin);
    layout->addWidget(pBtnThreshEdge);

    setLayout(layout);

    connect(pBtnSkin,&QPushButton::clicked,this,&chooseTraditionalDetectionTypeWidget::onSkinButtonClicked);
    connect(pBtnThreshEdge,&QPushButton::clicked,this,&chooseTraditionalDetectionTypeWidget::onThreshEdgeButtonClicked);
    }

void chooseTraditionalDetectionTypeWidget::onSkinButtonClicked(){
    selectedType = 0;
    emit typeSelected(0);
    accept();
}

void chooseTraditionalDetectionTypeWidget::onThreshEdgeButtonClicked(){
    selectedType = 1;
    emit typeSelected(1);
    accept();
}
