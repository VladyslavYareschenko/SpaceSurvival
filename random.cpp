#include "random.h"

int random(int min,int max)
{
    QTime *time = new QTime();
    qsrand(reinterpret_cast<uint>(time));
    delete time;
    return (qrand() % ((max + 1) - min) + min);
}
