#ifndef CGLWIDGET_H
#define CGLWIDGET_H

#include <QGLWidget>

class CGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    CGLWidget(QWidget *parent = 0);
    ~CGLWidget();
    void setData(int nObj, int *nLayers, int *nPtsPerLayer, float *Vertex);
    void createGeometry();
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* pe);
private:
    void draw();
    void draw1();
    void drawAxis();       // построить оси координат
    GLfloat rotX, rotY, rotZ;
    GLfloat dX, dY;
    GLfloat kScale;
    QColor faceColor[4];
    QPoint lastPos;
    QColor qtPurple;
    GLfloat *vertex;
    QVector3D *normals;
    int **faces;
    int n_obj;
    int *layers, *npts_on_layer, *szFaces, szVertex;

    void deleteGeometry();
};

#endif // CGLWIDGET_H
