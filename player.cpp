#include "player.h"
#include "asteroid.h"

Player::Player() : InteractingItem ()
{
    RotationAngle = 0;
    Health = 5;
    Score = 0;
    TimerIterationCounter = 0;
    outOfBordersCoursor = new TriangleCursor();
    timer = new QTimer();
    timer->start(15);
    connect(timer, &QTimer::timeout, this, &Player::controlSlot);
    connect(timer, &QTimer::timeout, this, &Player::bordersCheckSlot);
    connect(timer, &QTimer::timeout, this, &Player::collisionCheck);
}

Player::~Player()
{
    delete timer;
}

int Player::type() const
{
    return ClassType();
}

int Player::ClassType()
{
    return 70001;
}

int Player::score() const
{
    return this->Score;
}

void Player::setScore(int score)
{
    this->Score = score;
}

QRectF Player::boundingRect() const{
    return QRectF(-20,-20,40,40);

}

QPainterPath Player::shape() const
{
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->drawPixmap(boundingRect(), QPixmap(":/img/img/playerShip.png"), QRectF(0.0,0.0,46.0,32.0));
     Q_UNUSED(option);
     Q_UNUSED(widget);
}

void Player::controlSlot()
{
    TimerIterationCounter++;

    if(GetAsyncKeyState('A')){
            RotationAngle-= 3;
            this->setRotation(RotationAngle);
        }
    if(GetAsyncKeyState('D')){
            RotationAngle += 3;
            this->setRotation(RotationAngle);
        }
    if(GetAsyncKeyState('W')){
            this->setPos(mapToParent(0, -3));

        }
    if(GetAsyncKeyState('S')){
            this->setPos(mapToParent(0, 3));
        }
    if(GetAsyncKeyState(VK_SHIFT))
        if(TimerIterationCounter > 20)
            {
                Bullet * bullet = new Bullet(this);
                this->scene()->addItem(bullet);
                TimerIterationCounter = 0;
            }
}

void Player::bordersCheckSlot()
{
    double width = this->scene()->width();
    double heigth = this->scene()->height();

    if(this->x()<-5 && this->y()<-5)
    { outOfBordersCoursor->setTriangle(315, QPointF(5,5), this->scene()); }

    else if(this->x() > (width + 10) && this->y() < -5)
    { outOfBordersCoursor->setTriangle(45, QPointF(width, 5), this->scene()); }

    else if(this->x()> (width + 10) && this->y() > (heigth + 10))
    { outOfBordersCoursor->setTriangle(135, QPointF(width, heigth), this->scene()); }

    else if(this->x() < -5 && this->y() > (heigth + 10))
    { outOfBordersCoursor->setTriangle(225, QPointF(5, heigth), this->scene()); }

    else if(this->x() < -5)
    { outOfBordersCoursor->setTriangle(270, QPointF(0, this->y()), this->scene()); }

    else if(this->x() > (width + 10))
    { outOfBordersCoursor->setTriangle(90, QPointF(width, this->y()), this->scene()); }

     else if(this->y() > heigth)
    { outOfBordersCoursor->setTriangle(180, QPointF(this->x(), heigth), this->scene()); }

     else if(this->y() < -5)
    { outOfBordersCoursor->setTriangle(0, QPointF(this->x(), 0), this->scene()); }
     else
        if(outOfBordersCoursor->scene()!= nullptr)
          this->scene()->removeItem(outOfBordersCoursor);
}

void Player::hit()
{
    this->Health--;
    emit this->healthChanged();
    if(Health <= 0)
        emit this->dead();
}

void Player::collisionCheck()
{
    QList<QGraphicsItem *> collidingItems = this->collidingItems();
    if(collidingItems.count()>1)
    {
       for(auto GraphicItem : collidingItems)
       {
           if(GraphicItem->type() == Asteroid::ClassType())
           {
               GraphicItem->scene()->removeItem(GraphicItem);
               this->hit();
               dynamic_cast<Asteroid *>(GraphicItem)->deleteLater();
           }
       }
    }
}

Player::TriangleCursor::TriangleCursor(){}

QRectF  Player::TriangleCursor:: boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void Player::TriangleCursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygon polygon;
    polygon << QPoint(0,-15) << QPoint(12,15) << QPoint(-12,15);
    painter->setBrush(Qt::darkMagenta);
    painter->drawPolygon(polygon);
         Q_UNUSED(option);
         Q_UNUSED(widget);
}

void Player::TriangleCursor::setTriangle(int Rotation, const QPointF &Position, QGraphicsScene *scene)
{
    if(this->scene() == nullptr)
        scene->addItem(this);
    this->setPos(Position);
    this->setRotation(Rotation);
}
