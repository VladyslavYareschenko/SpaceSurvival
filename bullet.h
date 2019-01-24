#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>

class Bullet : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Bullet(QGraphicsItem * shooterObject);
    ~Bullet();
    QGraphicsItem * shooter() const;
    int type() const;
    static int classType();
signals:

public slots:
    void movingSlot();
private:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void hitCheck();
    short movingIterations;
    QGraphicsItem * Shooter;
};

#endif // BULLET_H
