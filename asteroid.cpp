#include "asteroid.h"
#include <QDebug>

static QPixmap * asteroidSpriteImage;
static QTimer * timer = new QTimer();

Asteroid::Asteroid() : QObject(), QGraphicsItem()
{
    movingIterations = 0;
    currentFrameX = 191;
    currentFrameY = 818;
    if(!timer->isActive())
        timer->start(15);
    connect(timer,&QTimer::timeout,this,&Asteroid::movingSlot);
}

int Asteroid::type() const
{
    return ClassType();
}

int Asteroid::ClassType()
{
    return 70004;
}

void Asteroid::loadSprite()
{
    asteroidSpriteImage = new QPixmap(":/img/img/asteroidsSet.png");
}

QRectF Asteroid::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void Asteroid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->drawPixmap(boundingRect(), *asteroidSpriteImage, QRectF(currentFrameX, currentFrameY, 39.0, 35.0));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath Asteroid::shape() const
{
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
}

void Asteroid::movingSlot()
{
    setPos(mapToParent(0,5));
    calculateNextFrame();
    movingIterations++;
    if(movingIterations > 150)
    {
        delete this;
    }
}

void Asteroid::calculateNextFrame()
{
    if(movingIterations % 4 == 0)
    {
        currentFrameX += 99;
        currentFrameY += 44;
        if(currentFrameX >= 1300 || currentFrameY >= 1330)
        {
            currentFrameX = 191; currentFrameY = 818;
        }
    }
}

void Asteroid::setRandomPosition()
{
    switch(random(1,4))
    {
    case 1:
        setPos(random(0, this->scene()->width()), -50);
        setRotation(random(-90, 90));
        return;
    case 2:
        setPos(this->scene()->width() + 50, random(0, this->scene()->height()));
        setRotation(random(0, 180));
        return;
    case 3:
        setPos(random(0, this->scene()->width()), this->scene()->height() + 50);
        setRotation(random(90, 270));
        return;
    case 4:
        setPos(-50,random(0, this->scene()->height()));
        setRotation(random(180, 360));
        return;
    default:
        return;
    }
}
