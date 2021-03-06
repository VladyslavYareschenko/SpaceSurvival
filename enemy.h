#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include "player.h"
#include "bullet.h"
#include "random.h"
#include "explosion.h"
#include "interactingitem.h"

class Enemy : public InteractingItem
{
    Q_OBJECT
public:
    explicit Enemy(Player * player, const QPixmap &enemyShipImage);
    int type() const;
    static int ClassType();
    bool setOnRandomPosition(QGraphicsScene * targetScene);
signals:
    void dead();
public slots:
    void hit();
    void crash();
private slots:
    void move();
    void shoot();
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPainterPath shape() const;
    void collisionCheck();
    int TimerIterationsCounter;
    double Rotation;
    Player *PlayerObject;
    QPixmap shipImage;
    class HealthBar:  public QObject,public QGraphicsItem
    {
    public:
        explicit HealthBar(Enemy *HealthBarHolder);
        ~HealthBar();
        void placeBesideHolder();
    private:
        Enemy *HolderObject;
        QTimer *timer;
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    };
    HealthBar * healthBar;
};

#endif // ENEMY_H
