#include "enemy.h"
#include "asteroid.h"
#include "QDebug"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2 * Pi;
static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}
static QTimer * timer = new QTimer();

Enemy::Enemy(Player * player, const QPixmap &enemyShipImage) : QObject(), QGraphicsItem(),
    PlayerObject(player),
    shipImage(enemyShipImage)
{
    Health = 3;
    healthBar = new HealthBar(this);
    Rotation = 0;
    TimerIterationsCounter = 0;
    if(!timer->isActive())
        timer->start(15);
    connect(timer,&QTimer::timeout,this,&Enemy::traectoryMoving);
    connect(timer,&QTimer::timeout,this,&Enemy::shoot);
    connect(timer,&QTimer::timeout,this,&Enemy::collisionCheck);
}

int Enemy::type() const
{
    return ClassType();
}

int Enemy::ClassType()
{
    return 70003;
}

int Enemy::health() const
{
    return Health;
}

void Enemy::GameOver()
{
    this->deleteLater();
}

QRectF Enemy::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->drawPixmap(boundingRect(), shipImage,QRectF(0,0,282.0,275.0));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QPainterPath Enemy::shape() const
{
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
}

void Enemy::traectoryMoving()
{
    QLineF lineToTarget(QPointF(0, 0), mapFromItem(PlayerObject, 0, 0));
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);
    if (angleToTarget > 0 && angleToTarget < Pi)
    {
        if(angleToTarget > Pi / 5)
        {
            Rotation = -1.2;
        } else if(angleToTarget > Pi / 10)
        {
            Rotation = -1;
        } else
        {
            Rotation = -0.5;
        }
    }
    else if (angleToTarget <= TwoPi && angleToTarget > (TwoPi - Pi)) {
        if(angleToTarget < (TwoPi - Pi / 5))
        {
            Rotation = 1.2;
        } else if(angleToTarget < (TwoPi - Pi / 10))
        {
            Rotation = 1;
        } else
        {
            Rotation = 0.5;
        }
    }
    else if(static_cast<int>(angleToTarget) == 0)
    {
        Rotation = 0;
    }
    setRotation(rotation() + Rotation);
    setPos(mapToParent(0,-1.5));
    healthBar->placeBesideHolder();
}

bool Enemy::setOnRandomPosition(QGraphicsScene * targetScene)
{
    int randomPosX = random(100, static_cast<int>(targetScene->width()) - 100);
    int randomPosY = random(100, static_cast<int>(targetScene->height()) - 100);
    QGraphicsEllipseItem * AreaAroundEnemy = new QGraphicsEllipseItem(-125, -125, 250, 250);
    targetScene->addItem(AreaAroundEnemy);
    AreaAroundEnemy->setPos(randomPosX, randomPosY);
    if(AreaAroundEnemy->collidingItems().count() <= 1)
    {
        targetScene->addItem(this);
        setPos(randomPosX, randomPosY);
        setRotation(random(0, 360));
        targetScene->removeItem(AreaAroundEnemy);
        this->scene()->addItem(healthBar);
        delete AreaAroundEnemy;
        return true;
     }
    targetScene->removeItem(AreaAroundEnemy);
     delete AreaAroundEnemy;
    return false;
}

void Enemy::shoot()
{
    TimerIterationsCounter++;
    if(TimerIterationsCounter > 30)
    {
        QPolygonF VisionPolygon;
        VisionPolygon << QPointF(this->x(), this->y())
                      << QPointF(this->x()-150, this->y()+250)
                      << QPointF(this->x()+150, this->y()+250);
        QMatrix matrix;
        matrix.translate(this->x(), this->y());
        matrix.rotate(this->rotation()+180);
        matrix.translate(-this->x(), -this->y());
        VisionPolygon = matrix.map(VisionPolygon);
        if(PlayerObject != nullptr)
        {
        if(VisionPolygon.containsPoint(PlayerObject->pos(), Qt::OddEvenFill))
        {
            Bullet * bullet = new Bullet(this);
            this->scene()->addItem(bullet);
        }
        }
        TimerIterationsCounter = 0;
    }
}

void Enemy::hit()
{
    this->Health--;
    if(Health == 0)
    {
        this->crash();
        this->deleteLater();
    }
}

void Enemy::crash()
{
    Explosion * exp = new Explosion(this->pos());
    this->scene()->addItem(exp);
    this->scene()->removeItem(healthBar);
    this->scene()->removeItem(this);
    healthBar->update();
    emit this->dead();
}

void Enemy::collisionCheck()
{
    QList<QGraphicsItem *> collidingItems = this->collidingItems();
    if(collidingItems.count()>1)
    {
       for(auto GraphicItem : collidingItems)
       {
           if(GraphicItem == PlayerObject)
           {
               this->crash();
               PlayerObject->hit();
               this->deleteLater();
           }
           if(GraphicItem->type() == Asteroid::ClassType())
           {
               GraphicItem->scene()->removeItem(GraphicItem);
               this->crash();
               this->deleteLater();
               dynamic_cast<Asteroid *>(GraphicItem)->deleteLater();
           }
           if(GraphicItem->type() == Enemy::ClassType())
           {
               this->crash();
               emit dynamic_cast<Enemy *>(GraphicItem)->crash();
               delete GraphicItem;
               this->deleteLater();
           }
       }
    }
}

Enemy::HealthBar::HealthBar(Enemy *HealthBarHolder) : HolderObject(HealthBarHolder) {}

Enemy::HealthBar::~HealthBar() {}

QRectF Enemy::HealthBar::boundingRect() const
{
    return QRectF(-10,-30,10,30);
}

void Enemy::HealthBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());
    painter->setBrush(Qt::red);
    painter->drawRect(QRectF(-10,-((HolderObject->health())*10),10,((HolderObject->health())*10)));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Enemy::HealthBar::placeBesideHolder()
{
    this->setPos(HolderObject->x()+35,HolderObject->y()+10);
}

