#ifndef ASTEROID_H
#define ASTEROID_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>
#include "random.h"

class Asteroid : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Asteroid();
    int type() const;
    static int ClassType();
    void setRandomPosition();
    static void loadSprite();
signals:

public slots:
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPainterPath shape() const;
    short movingIterations;
    int currentFrameX;
    int currentFrameY;
private slots:
    void movingSlot();
    void calculateNextFrame();
};

#endif // ASTEROID_H
