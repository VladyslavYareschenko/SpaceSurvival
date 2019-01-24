#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>

class Explosion : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Explosion(const QPointF &position);
signals:

public slots:
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPainterPath shape() const;
    double currentFrameX;
    double currentFrameY;
    double X;
    double Y;
    short frameIterations;
    void calculateNextFrame();
};

#endif // EXPLOSION_H
