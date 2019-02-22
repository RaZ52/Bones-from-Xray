#include "cglwidget.h"
#include <QMouseEvent>
#include <math.h>
#include <QVector3D>

CGLWidget::CGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    rotX = -21.0;
    rotY = -57.0;
    rotZ = 0.0;
    dX = 0.0;
    dY = 0.0;
    kScale = 1.0;
//    faceColor = Qt::green;
    faceColor[0] = Qt::green;
    faceColor[1] = Qt::red;
    faceColor[2] = Qt::yellow;
    faceColor[3] = Qt::blue;

    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

CGLWidget::~CGLWidget()
{
    deleteGeometry();
}

void CGLWidget::setData(int nObj, int *nLayers, int *nPtsPerLayer, float *Vertex)
{
    n_obj = nObj;
    layers = nLayers;
    npts_on_layer = nPtsPerLayer;
    szVertex = 0;
    for (int i = 0; i < n_obj; i++)   szVertex += 3 * layers[i] * npts_on_layer[i];
    vertex = Vertex;
}

void CGLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
//    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 100.5, 105.0, 120.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
 //   glLightfv( GL_LIGHT0, GL_AMBIENT, lightPosition );

}

void CGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(dX, dY, -10.0);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotZ, 0.0, 0.0, 1.0);
    glScalef(kScale, kScale, kScale);
 //   drawAxis();
    draw1();
}

void CGLWidget::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->pos();
}

void CGLWidget::mouseMoveEvent(QMouseEvent *event)
{

    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
    if (event->buttons() & Qt::LeftButton) {
        rotX += 180 * dy;
        rotY += 180 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        rotX += 180 * dy;
        rotZ += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();

}

void CGLWidget::wheelEvent(QWheelEvent *pe)
{
    if ((pe->delta())>0) kScale *= 1.1;
    else if ((pe->delta())<0) kScale /= 1.1;
    updateGL(); // обновление изображения
}

void CGLWidget::draw()
{
    static const GLfloat P1[3] = { 0.0, -1.0, +2.0 };
    static const GLfloat P2[3] = { +1.73205081, -1.0, -1.0 };
    static const GLfloat P3[3] = { -1.73205081, -1.0, -1.0 };
    static const GLfloat P4[3] = { 0.0, +2.0, 0.0 };
    static const GLfloat * const coords[4][3] = {
        { P1, P2, P3 }, { P1, P3, P4 }, { P1, P4, P2 }, { P2, P4, P3 }
    };

    for (int i = 0; i < 4; ++i) {
        glLoadName(i);
        glBegin(GL_TRIANGLES);
        qglColor(faceColor[i]);
        for (int j = 0; j < 3; ++j) {
            glVertex3f(coords[i][j][0], coords[i][j][1], coords[i][j][2]);
        }
        glEnd();
    }
}

void CGLWidget::draw1() {

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertex);// Указываем массив вершин.
    glNormalPointer(GL_FLOAT, 0, normals);
    // Передаём массив индексов вершин.
    // Первый параметр - тип примитивов (у нас 2 треугольника), второй -
    // количество полигонов * количество вершин в примитиве. В нашем случае -
    // 2 полигона * 3 точки. У GL_QUADS было бы 2*4, и т.п.
    // Далее передаём тип переменной (GL_UNSIGNED_INT), и наконец массив индексов
    // вершин (полигонов.
    for (int i = 0; i < n_obj; i ++) {
        glDrawElements(GL_TRIANGLES,szFaces[i],GL_UNSIGNED_INT, faces[i]);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void CGLWidget::drawAxis()
{
    // устанавливаем ширину линии приближенно в пикселях
       glLineWidth(3.0f);
       // до вызова команды ширина равна 1 пикселю по умолчанию

       // устанавливаем цвет последующих примитивов
       glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
       // ось x красного цвета
       glBegin(GL_LINES); // построение линии
          glVertex3f( 1.0f,  0.0f,  0.0f); // первая точка
          glVertex3f(-1.0f,  0.0f,  0.0f); // вторая точка
       glEnd();

       QColor halfGreen(0, 128, 0, 255);
       qglColor(halfGreen);
       glBegin(GL_LINES);
          // ось y зеленого цвета
          glVertex3f( 0.0f,  1.0f,  0.0f);
          glVertex3f( 0.0f, -1.0f,  0.0f);

          glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
          // ось z синего цвета
          glVertex3f( 0.0f,  0.0f,  1.0f);
          glVertex3f( 0.0f,  0.0f, -1.0f);
       glEnd();
}



void CGLWidget::createGeometry()
{

    normals = new QVector3D[szVertex/3]; for (int i = 0; i < szVertex/3; i++) normals[i] = QVector3D(0,0,0);
    szFaces = new int[n_obj];
    faces = new int*[n_obj];

    for (int i = 0; i < n_obj; i++) {
        szFaces[i] = 3 * 2 * npts_on_layer[i]*(layers[i]-1);
        faces[i] = new int [szFaces[i]];
    }

    int shift = 0;
    for (int k = 0; k< n_obj; k++){
        int counter = 0;
        for (int i = 0; i < layers[k] - 1; i++){
            for (int j = 0; j < npts_on_layer[k]; j++){
                faces[k][counter] = shift + i * npts_on_layer[k] + j;
                QVector3D v1 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;
                faces[k][counter] = shift + i * npts_on_layer[k] + (j + 1)%npts_on_layer[k];
                QVector3D v2 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;
                faces[k][counter] = shift + (i+1) * npts_on_layer[k] + (j + 1)%npts_on_layer[k];
                QVector3D v3 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;

                QVector3D normal1 = QVector3D::normal(v1,v2,v3);

                normals[faces[k][counter-3]] += normal1;
                normals[faces[k][counter-2]] += normal1;
                normals[faces[k][counter-1]] += normal1;

                faces[k][counter] = shift + i * npts_on_layer[k] + j;
                QVector3D v4 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;
                faces[k][counter] = shift + (i+1) * npts_on_layer[k] + (j+1)%npts_on_layer[k];
                QVector3D v5 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;
                faces[k][counter] = shift + (i+1) * npts_on_layer[k] + j;
                QVector3D v6 = QVector3D(vertex[faces[k][counter]*3], vertex[faces[k][counter]*3 + 1], vertex[faces[k][counter]*3 + 2]);
                counter++;

                QVector3D normal2 = QVector3D::normal(v4,v5,v6);

                normals[faces[k][counter-3]] += normal2;
                normals[faces[k][counter-2]] += normal2;
                normals[faces[k][counter-1]] += normal2;
            }
        }
        shift+=npts_on_layer[k]*layers[k];
    }
    for (int i = 0; i < szVertex/3; i++) {
        normals[i] = normals[i].normalized();
    }
}

void CGLWidget::deleteGeometry()
{
    delete[] faces;
 //   delete[] vertex;
    delete[] normals;

}
