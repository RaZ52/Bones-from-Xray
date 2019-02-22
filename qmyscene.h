#ifndef QMYSCENE_H
#define QMYSCENE_H

#include <QtWidgets>
#include <QWidget>

class QMyScene : public QGraphicsScene
{
private:
    int cont_cnt;
    bool is_set_pt;
    int n_color;
    QVector<QGraphicsItem*> items;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
    QMyScene(const QRectF &);
    QVector<QPointF> m_points;
    QVector<int> i_beg;
    QVector<int> i_end;
    void clearContours();
};

#endif // QMYSCENE_H
