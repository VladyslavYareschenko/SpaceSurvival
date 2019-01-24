#ifndef INTERACTINGITEM_H
#define INTERACTINGITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <explosion.h>

class InteractingItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    virtual int health() const;
signals:
    virtual void dead();
public slots:
    virtual void hit() = 0;
    virtual void crash();
protected:
    int Health;
};

#endif // INTERACTINGITEM_H
