#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QtWidgets>
#include <QWidget>
#include "crgpic.h"

class CMainWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton *render_bt;
    CRgPic *rg_left, *rg_right;
    int nObj, *nLayers, *nPtsPerLayer;
    float *Vertex;
    bool calc3D();
    void test3D();
private slots:
    void renderClick();
public:
    CMainWidget(QWidget *parent = 0);
    ~CMainWidget();
};

#endif // CMAINWIDGET_H
