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

class Enemy : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Enemy(Player * player, const QPixmap &enemyShipImage);
    int type() const;
    static int ClassType();
    int health() const;
    bool setOnRandomPosition(QGraphicsScene * targetScene);
    void hit();
    void crash();
signals:
    void dead();
public slots:
    void GameOver();
private slots:
    void traectoryMoving();
    void shoot();
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPainterPath shape() const;
    void collisionCheck();
    int Health;
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
