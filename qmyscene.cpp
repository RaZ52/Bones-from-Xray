#include "qmyscene.h"


void QMyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsRectItem *item = new QGraphicsRectItem;
        QGraphicsTextItem *text = new QGraphicsTextItem;
        QGraphicsTextItem *text2 = new QGraphicsTextItem;
        item->setRect(QRectF(-1.5,-1.5,1.5,1.5));
        item->setPos(event->scenePos());
        n_color = 6+cont_cnt;
        if (!is_set_pt) n_color++;
        item->setPen(QPen(QColor(Qt::GlobalColor(n_color))));
        addItem(item);
        items.append(item);
        m_points.append(event->scenePos());
        int cnt = m_points.size() - 1;
        if ( is_set_pt ) {
            QGraphicsLineItem *line = new QGraphicsLineItem;
            line->setLine(QLineF(m_points[cnt-1], m_points[cnt]));
            line->setPen(QPen(QColor(Qt::GlobalColor(n_color))));
            text->setPlainText(QString::number(m_points[cnt].x()));
            text->setPos(event->scenePos() - QPointF(0,10));
            text2->setPlainText(QString::number(m_points[cnt].y()));
            text2->setPos(event->scenePos());
            addItem(text2);
            addItem(text);
            items.append(text);
            items.append(text2);
            addItem(line);
            items.append(line);
        }
        else {
            cont_cnt++;
            i_beg.append(cnt);
        }
        is_set_pt = true;
    }
    if (event->button() == Qt::RightButton && is_set_pt) {
        i_end.append(m_points.size() - 1);
        QGraphicsLineItem *line = new QGraphicsLineItem;
        line->setLine(QLineF(m_points[i_beg[cont_cnt-1]], m_points[i_end[cont_cnt-1]]));
        line->setPen(QPen(QColor(Qt::GlobalColor(n_color))));
        addItem(line);
        items.append(line);
        is_set_pt = false;
    }

}


void QMyScene::clearContours()
{
    m_points.clear();
    i_beg.clear();
    i_end.clear();
    cont_cnt = 0;
    is_set_pt = false;
    foreach (QGraphicsItem *itm, items) removeItem(itm);
    items.clear();
}


QMyScene::QMyScene(const QRectF &rect)  : QGraphicsScene(rect)
{
    cont_cnt = 0;
    is_set_pt = false;
}
