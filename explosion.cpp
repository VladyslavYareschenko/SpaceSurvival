#include "explosion.h"

static QPixmap * explosionSpriteImage;
static QTimer * timer = new QTimer();

Explosion::Explosion(const QPointF &position) : QObject(), QGraphicsItem()
{
    currentFrameX = 10;
    currentFrameY = 10;
    frameIterations = 0;
    this->setPos(position);
    if(explosionSpriteImage == nullptr)
        explosionSpriteImage = new QPixmap(":/img/img/EnemysShipExplosion.png");
    if(!timer->isActive())
        timer->start(40);
     connect(timer,&QTimer::timeout,this,&Explosion::calculateNextFrame);
}

QRectF Explosion::boundingRect() const
{
    return QRectF(-30,-30,60,60);
}

void Explosion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->drawPixmap(boundingRect(), *explosionSpriteImage, QRectF(currentFrameX, currentFrameY, 80.0, 80.0));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath Explosion::shape() const
{
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
}

void Explosion::calculateNextFrame()
{
    currentFrameX += 100;
    if(frameIterations % 10 == 0)
    {
    currentFrameY +=100;
    currentFrameX = 10;
    }
    frameIterations++;
    this->update();
    if(frameIterations > 50)
    {
        this->scene()->removeItem(this);
        delete this;
    }

}
