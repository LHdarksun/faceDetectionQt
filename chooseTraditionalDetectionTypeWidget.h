#ifndef CHOOSEFACEDETECTIONTYPEWIDGET_H
#define CHOOSEFACEDETECTIONTYPEWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class chooseTraditionalDetectionTypeWidget : public QDialog{
    Q_OBJECT;

public:
    explicit chooseTraditionalDetectionTypeWidget(QWidget *parent = nullptr);
    int getSelectedType() const {return selectedType;}
signals:
    //0:肤色分割        1:阈值+边缘分割
    void typeSelected(int type);
private slots:
    void onSkinButtonClicked();
    void onThreshEdgeButtonClicked();
private:
    QPushButton *pBtnSkin;
    QPushButton *pBtnThreshEdge;
    int selectedType = 0;
};

#endif // CHOOSEFACEDETECTIONTYPEWIDGET_H
//虽然类名中有Widget，但是实际实现是继承的QDialog
