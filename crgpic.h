#ifndef CRGPIC_H
#define CRGPIC_H

#include <QtWidgets>
#include <QWidget>
#include "qmyscene.h"


class CRgPic : public QWidget
{
    Q_OBJECT
private:
    QGraphicsView *view;
    QPushButton *open_bt, *clear_bt;
    bool loadFile(const QString &);
private slots:
    void loadPic();
    void clearAll();

public:
    QMyScene *scene;
    CRgPic(QWidget *parent = 0);
    ~CRgPic();
};


#endif // CRGPIC_H
