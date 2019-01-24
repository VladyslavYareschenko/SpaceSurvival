#include "bullet.h"
#include "enemy.h"
#include "player.h"

static QTimer * timer = new QTimer();

Bullet::Bullet(QGraphicsItem * shooterObject) :
    Shooter(shooterObject),
    QObject(),
    QGraphicsItem()
{
    setPos(Shooter->pos());
    setRotation(Shooter->rotation() + 180);
    movingIterations = 0;
    if(!timer->isActive())
        timer->start(10);
    connect(timer,&QTimer::timeout,this,&Bullet::movingSlot);
}

Bullet::~Bullet()
{
}

QRectF Bullet::boundingRect() const
{
    return QRectF(-4,-4,8,8);

}

QPainterPath Bullet::shape() const
{
     QPainterPath path;
     path.addEllipse(boundingRect());
     return path;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     painter->setBrush(Qt::green);
     painter->drawEllipse(boundingRect());
     Q_UNUSED(option);
     Q_UNUSED(widget);
}

int Bullet::type() const
{
    return 70002;
}

int Bullet::classType()
{
    return 70002;
}

QGraphicsItem * Bullet::shooter() const
{
    return Shooter;
}

void Bullet::movingSlot()
{

    movingIterations++;
    setPos(mapToParent(0,7));
    QList<QGraphicsItem *> collidingItems = this->collidingItems();

    collidingItems.removeOne(this->shooter());
    if(collidingItems.count()>1)
    {
        for(auto GraphicObject : collidingItems)
        {
            if(GraphicObject->type() == Enemy::ClassType())
            {
                dynamic_cast<Enemy *>(GraphicObject)->hit();
                this->deleteLater();
            }
            if(GraphicObject->type() == Player::ClassType())
            {
                dynamic_cast<Player *>(GraphicObject)->hit();
                this->deleteLater();
            }
        }
    }
    if(movingIterations > 100)
    {
        this->deleteLater();
    }
}

