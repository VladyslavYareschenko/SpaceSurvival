#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTime>
#include "player.h"
#include "enemy.h"
#include "asteroid.h"
#include "explosion.h"
#include "bullet.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    QGraphicsScene *playFieldScene;
    QGraphicsScene *healthBarScene;
    Player * player;
    QTimer * timer;
    short timerIterationsCounter;
    short enemyCounter;
    QPixmap enemyShipImage;
    bool GameStarted;
private slots:
    void createEnemy();
    void createAsteroid();
    void drawHearts();
    void changeScore();
    void reduceEnemyCounter();
    void startGame();
    void gameOver();
    void addWelcomeText();
};

#endif // MAINWIDGET_H
