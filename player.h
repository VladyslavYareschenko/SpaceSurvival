#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include "Windows.h"
#include "bullet.h"

#define PLAYER_TYPE 70001

class Player : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Player();
    ~Player();
    int type() const;
    static int ClassType();
    int health() const;
    int score() const;
    void setScore(int score);
signals:
    void dead();
    void healthChanged();
public slots:
    void hit();
private slots:
    void controlSlot();
    void bordersCheckSlot();
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPainterPath shape() const;
    void collisionCheck();
    int RotationAngle;
    int Health;
    int Score;
    int TimerIterationCounter;
    QTimer * timer;
    class TriangleCursor: public QGraphicsItem
    {
    public:
        TriangleCursor();
        void setTriangle(int Rotation, const QPointF &Position, QGraphicsScene *scene);
    private:
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    };
    TriangleCursor * outOfBordersCoursor;
};

#endif // PLAYER_H
