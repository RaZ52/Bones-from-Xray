#include "cmainwidget.h"
#include "cglwidget.h"
#include <math.h>

bool CMainWidget::calc3D()
{
    QVector<int> *beg_l = &(rg_left->scene->i_beg);
    QVector<int> *beg_r = &(rg_right->scene->i_beg);
    QVector<int> *end_l = &(rg_left->scene->i_end);
    QVector<int> *end_r = &(rg_right->scene->i_end);
    int ncl = beg_l->size();
    int ncr = beg_r->size();
    if (ncl == 0 || ncl != ncr) {
        QMessageBox::critical(NULL,"Error","Error in contour definition");
        return false;
    }


    QVector<QPointF> pts_l = rg_left->scene->m_points;
    QVector<QPointF> pts_r = rg_right->scene->m_points;

    //1. подгон координат
    QPointF left_max, left_min, right_max, right_min,  amax, amin, d;
    float lxmax, lxmin, lymax, lymin, rymax, rymin, rxmax, rxmin;
    lxmax = pts_l[0].x();
    lxmin = pts_l[0].x();
    lymax = pts_l[0].y();
    lymin = pts_l[0].y();
    rxmax = pts_r[0].x();
    rxmin = pts_r[0].x();
    rymax = pts_r[0].y();
    rymin = pts_r[0].y();

    for(int i = 0; i < pts_l.size(); i++)
    {
        if (pts_l[i].y() > lymax) {
            lymax =  pts_l[i].y();
        }
        if (pts_l[i].y() < lymin) {
            lymin = pts_l[i].y();
        }
        if (pts_l[i].x() > lxmax) {
            lxmax =  pts_l[i].x();
        }
        if (pts_l[i].x() < lxmin) {
            lxmin = pts_l[i].x();
        }
    }
    for(int i = 0; i < pts_r.size(); i++)
    {
        if (pts_r[i].y() > rymax) {
            rymax =  pts_r[i].y();
        }
        if (pts_r[i].y() < rymin) {
            rymin = pts_r[i].y();
        }
        if (pts_r[i].x() > rxmax) {
            rxmax =  pts_r[i].x();
        }
        if (pts_r[i].x() < rxmin) {
            rxmin = pts_r[i].x();
        }
    }


    //подгоним координаты ///////////////////////
    //сместить все координаты всех контуров на снимках в центр 0,0
    float lcy = (lymin + lymax) / 2;
    float rcy = (rymin + rymax) / 2;
    float lcx = (lxmin + lxmax) / 2;
    float rcx = (rxmin + rxmax) / 2;

    //float keff = .1;
    for(int i = 0;i < pts_l.size();i++)
    {
        pts_l[i].setY((pts_l[i].y() - lcy));
        pts_l[i].setX((pts_l[i].x() - lcx));
    }

    for(int i = 0;i < pts_r.size();i++)
    {
        pts_r[i].setY((pts_r[i].y() - rcy));
        pts_r[i].setX((pts_r[i].x() - rcx));
    }


    // ////////////////////////////
    nObj = ncl;
    nLayers = new int[nObj];
    nPtsPerLayer = new int[nObj];
    for (int i = 0; i < nObj; i++) {
        nLayers[i] = 10;
        nPtsPerLayer[i] = 32;
    }
    int szVertex = 0;
    for (int i = 0; i<nObj; i++){
        szVertex += 3 * nLayers[i] * nPtsPerLayer[i];
    }
    Vertex = new float[szVertex];

    int counter = 0;
    for (int k = 0; k < nObj; k++){

        int i0_l = beg_l->at(k);
        int i0_r = beg_r->at(k);
        int in_l = end_l->at(k);
        int in_r = end_r->at(k);

        left_max = pts_l[i0_l];
        left_min = pts_l[i0_l];
        right_max = pts_r[i0_r];
        right_min = pts_r[i0_r];
        for(int i = i0_l; i <= in_l; i++)
        {
            if (pts_l[i].y() > left_max.y())  {
                left_max = QPointF(pts_l[i].x(), pts_l[i].y());
            }
            if (pts_l[i].y() < left_min.y())  {
                left_min = QPointF(pts_l[i].x(), pts_l[i].y());
            }

        }
        for(int i = i0_r; i <= in_r; i++)
        {
            if (pts_r[i].y() > right_max.y()) {
                right_max = QPointF(pts_r[i].x(), pts_r[i].y());
            }
            if (pts_r[i].y() < right_min.y()) {
                right_min = QPointF(pts_r[i].x(), pts_r[i].y());
            }
        }
        if(left_max.y() > right_max.y())   {
            amax = right_max ;
        }
        else  {
            amax = left_max ;
        }
        if(left_min.y() > right_min.y())  {
            amin = left_min;
        }
        else  {
            amin = right_min;
        }

        //????
        //\///////////////////////////////////////////
        //определим число слоёв и координату z
        float dz;
        QVector<float> z;
        dz = (amax.y() - amin.y()) / nLayers[k];

        for(int i = 0; i < nLayers[k]; i++)
        {
            float zz = amin.y() + dz / 2 + dz * i;
            z.append(zz);
        }


        //QVector<float> xc,rx,yc,ry;
        QVector<QPointF> xz,yz;

        for(int kk = 0; kk < z.size();kk++)
        {
            for(int i = i0_l; i <= in_l; i++)
            {
                int i0 = i-1;
                int i1 = i;
                if (i0 < i0_l) i0 = in_l;
                if(((pts_l[i0].y() - z[kk]) * (pts_l[i1].y() - z[kk])) < 0)
                {
                   xz.append(QPointF((pts_l[i0].x() + ((z[kk] - pts_l[i0].y()) * ((pts_l[i1].x() - pts_l[i0].x()) / (pts_l[i1].y() - pts_l[i0].y())))), z[kk]));
                }
            }
            for(int i = i0_r; i <= in_r; i++)
            {
                int i0 = i-1;
                int i1 = i;
                if (i0 < i0_r) i0 = in_r;
                if(((pts_r[i0].y() - z[kk]) * (pts_r[i1].y() - z[kk])) < 0)
                {
                   yz.append(QPointF((pts_r[i0].x() + ((z[kk] - pts_r[i0].y()) * ((pts_r[i1].x() - pts_r[i0].x()) / (pts_r[i1].y() - pts_r[i0].y())))), z[kk]));
                }
            }
        }
        // /////////////////////////////
        QVector<float> xc,rx,yc,ry;
        float alpha;
        float pi2;
        pi2 = 2. * 3.1415926535;
        alpha = pi2 / nPtsPerLayer[k];
        for(int i = 0; i < (xz.size() / 2);i++)
        {
            xc.append((xz[2*i+1].x() + xz[2*i].x()) / 2);
            rx.append(abs((xz[2*i+1].x() - xz[2*i].x()) / 2));
        }
        for(int i = 0; i < (yz.size() / 2);i++)
        {
            yc.append((yz[2*i+1].x() + yz[2*i].x()) / 2);
            ry.append(abs((yz[2*i+1].x() - yz[2*i].x()) / 2));
        }

        //float xkn[xz.size()][nPtsPerLayer[0]], ykn[yz.size()][nPtsPerLayer[0]];
        float xkn[100][100], ykn[100][100];
        for(int i = 0; i < nLayers[k]; i++)
        {
            for(int kk = 0; kk < nPtsPerLayer[k]; kk++)
            {
                xkn[i][kk] = xc[i] + rx[i] * cos(alpha * kk);
                ykn[i][kk] = yc[i] + ry[i] * sin(alpha * kk);
            }
        }


        for (int i = 0; i < nLayers[k]; i++){
            for (int j = 0; j < nPtsPerLayer[k]; j++){
               // float angle = pi2 / nPtsPerLayer[k] * j;
                Vertex[counter] = xkn[i][j];// -left_min.x();
                counter++;
                Vertex[counter] = ykn[i][j];//-right_min.x();
                counter++;
                Vertex[counter] = z[i];
                counter++;
            }
        }

    }

    /*
    for(int i = 0;i < pts_l.size() - 1;i++)
    {
        for(int k = 0;k < z.size();k++)
        {
            if(((pts_l[i].y() < z[k]) && (z[k] > pts_l[i+1].y())) || ((pts_l[i].y() > z[k]) && (z[k] < pts_l[i+1].y())))
            {
                xz.append(QPointF((pts_l[i].x() + ((z[k] - pts_l[i].y()) * ((pts_l[i+1].x() - pts_l[i].x()) / (pts_l[i+1].y() - pts_l[i].y())))), z[k]));
            }
        }
    }
    for(int i = 0;i < pts_r.size() - 1;i++)
    {
        for(int k = 0;k < z.size();k++)
        {
            if(((pts_r[i].y() < z[k]) && (z[k] > pts_r[i+1].y())) || ((pts_r[i].y() > z[k]) && (z[k] < pts_l[i+1].y())))
            {
                yz.append(QPointF((pts_r[i].x() + ((z[k] - pts_r[i].y()) * ((pts_r[i+1].x() - pts_r[i].x()) / (pts_r[i+1].y() - pts_r[i].y())))), z[k]));
            }
        }
    }
    */
/*

    ////////////////////////////////////////
    /// Debug Info
    ////////////////////////////////////////
    QListWidget *listw = new QListWidget;
    listw->addItem(QString::number(right_max.x()));
    listw->addItem(QString::number(right_max.y()));
    listw->addItem(QString::number(right_min.x()));
    listw->addItem(QString::number(right_min.y()));
    listw->addItem(QString::number(xz.size()));
    listw->addItem(QString::number(yz.size()));
    listw->addItem(QString::number(z.size()));
   // rg_left->update();
   // rg_right->update();
    listw->show();
    ////////////////////////////////////////
*/
    //test3D();
    /*
    nObj = ncl;
    nPtsPerLayer = new int[nObj];

    for (int i = 0; i < nObj; i++) nPtsPerLayer[i] = 16;

*/

    return true;
}

void CMainWidget::test3D()
{
    nObj = 2;
    nLayers = new int[nObj];
    nPtsPerLayer = new int[nObj];

    nLayers[0] = 4;
    nPtsPerLayer[0] = 16;

    nLayers[1] = 5;
    nPtsPerLayer[1] = 32;

    int szVertex = 0;
    for (int i = 0; i<nObj; i++){
        szVertex += 3 * nLayers[i] * nPtsPerLayer[i];
    }

    Vertex = new float[szVertex];
    float R = 0.5;
    float pi2 = 2. * 3.14159265;

    int counter = 0;
    for (int k = 0; k < nObj; k++){
        for (int i = 0; i < nLayers[k]; i++){
            for (int j = 0; j < nPtsPerLayer[k]; j++){
                float angle = pi2 / nPtsPerLayer[k] * j;
                Vertex[counter] = 2.*k + (R + (.1 * (i%2))) * cos(angle);
                counter++;
                Vertex[counter] = (R + (.3 * (i%2))) * sin(angle);
                counter++;
                Vertex[counter] = (-nLayers[k]/2 + i) * .7;
                counter++;
            }
        }
    }

}

void CMainWidget::renderClick()
{
    if (calc3D()) {
        CGLWidget *myGlWidget = new CGLWidget();
        myGlWidget->setData(nObj, nLayers, nPtsPerLayer, Vertex);
        myGlWidget->createGeometry();
        myGlWidget->show();
    }
}

CMainWidget::CMainWidget(QWidget *parent) : QWidget(parent)
{
    render_bt = new QPushButton(tr("&Render"));
    rg_left = new CRgPic;
    rg_right = new CRgPic;
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(rg_left);
    h_layout->addWidget(rg_right);

    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addLayout(h_layout);
    v_layout->addWidget(render_bt);

    setLayout(v_layout);

    connect(render_bt, SIGNAL(clicked()), this, SLOT(renderClick()));

}


CMainWidget::~CMainWidget(){}
