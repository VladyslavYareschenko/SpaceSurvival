#include "interactingitem.h"

int InteractingItem::health() const
{
    return Health;
}

void InteractingItem::crash()
{
    Explosion * exp = new Explosion(this->pos());
    this->scene()->addItem(exp);
    emit this->dead();
}
