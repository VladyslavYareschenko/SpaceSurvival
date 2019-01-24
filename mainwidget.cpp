#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "QDebug"

MainWidget::MainWidget(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWidget)
{
    GameStarted = false;
    ui->setupUi(this);
    this->setFixedSize(625,625);
    this->setStyleSheet("background-color: gray;");

    /* Constructing GraphicsViews;
       placing GraphicsScene`s on Views */
    ui->playFieldGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playFieldGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playFieldGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->playFieldGraphicsView->setFixedSize(605, 540);
    playFieldScene = new QGraphicsScene();
    playFieldScene->setSceneRect(0, 0, 605, 535);
    ui->playFieldGraphicsView->setScene(playFieldScene);

    ui->healthBarGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->healthBarGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->healthBarGraphicsView->setRenderHint(QPainter::Antialiasing);
    ui->healthBarGraphicsView->setFixedSize(260, 40);
    ui->healthBarGraphicsView->setStyleSheet(" QGraphicsView#healthBarGraphicsView "
                                      "{ background: transparent; "
                                      "border-style: transparent; }");
    healthBarScene = new QGraphicsScene();
    ui->healthBarGraphicsView->setScene(healthBarScene);
    QPixmap background(":/img/img/playFieldBackground.png");
    playFieldScene->addPixmap(background.scaled(playFieldScene->width(),playFieldScene->height()));
    addWelcomeText();
    connect(ui->startButton,&QPushButton::pressed,this,&MainWidget::startGame);

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::createEnemy()
{
    if(enemyCounter < 3)
    {
    Enemy * enemy = new Enemy(player, enemyShipImage);
    if(enemy->setOnRandomPosition(playFieldScene))
    {
        enemyCounter++;
        connect(enemy,&Enemy::dead,this,&MainWidget::changeScore);
        connect(enemy,&Enemy::dead,this,&MainWidget::reduceEnemyCounter);
        return;
    }
    else
        delete enemy;
    }
}

void MainWidget::createAsteroid()
{
    timerIterationsCounter++;
    if(!(timerIterationsCounter %= 2))
    {
    Asteroid * asteroid = new Asteroid();
    playFieldScene->addItem(asteroid);
    asteroid->setRandomPosition();
    }
}

void MainWidget::drawHearts()
{
    this->healthBarScene->clear();
    for(int i = 0; i < player->health(); ++i)
    {
        QGraphicsPixmapItem * heartItem = new QGraphicsPixmapItem(QPixmap(":/img/img/heart.png"));
        this->healthBarScene->addItem(heartItem);
        heartItem->setScale(0.15);
        heartItem->setPos(0.15*256*i+i*5,heartItem->y());
    }
    if(player->health() <= 0)
        gameOver();
}

void MainWidget::changeScore()
{
    player->setScore(player->score() + 1);
    this->ui->scoreLabel->setText("Score: "+ QString::number(player->score()));
}

void MainWidget::reduceEnemyCounter()
{
    this->enemyCounter--;
}

void MainWidget::startGame()
{
    if(!GameStarted)
    {
        enemyCounter = 0;
        playFieldScene->clear();
        QPixmap background(":/img/img/playFieldBackground.png");
        this->ui->scoreLabel->setText("Score: "+ QString::number(0));
        playFieldScene->addPixmap(background.scaled(playFieldScene->width(),playFieldScene->height()));
        player = new Player();
        playFieldScene->addItem(player);
        player->setPos(playFieldScene->width()/2, playFieldScene->height()/2);
        drawHearts();
        connect(player,&Player::healthChanged,this,&MainWidget::drawHearts);
        Asteroid::loadSprite();
        enemyShipImage = QPixmap(":/img/img/enemyShip.png");
        timer = new QTimer();
        timer->start(500);
        connect(timer,&QTimer::timeout,this,&MainWidget::createEnemy);
        connect(timer,&QTimer::timeout,this,&MainWidget::createAsteroid);
        GameStarted = true;
    }
}

void MainWidget::gameOver()
{
    disconnect(timer,&QTimer::timeout,this,&MainWidget::createEnemy);
    disconnect(timer,&QTimer::timeout,this,&MainWidget::createAsteroid);
    for(auto item : playFieldScene->items()){
        if(item->type() != Bullet::classType() && item != player && item->type() != 7)  // 7 == QPixmap background type
        delete item;
    }
    player->deleteLater();
    playFieldScene->removeItem(player);
    QGraphicsTextItem * text = playFieldScene->addText("You lost!");
    text->setFont(QFont("BankGothic Md BT", 22));
    text->setDefaultTextColor(QColor(Qt::red));
    text->setTextWidth(550);
    QFontMetrics fm(text->font());
    text->setPos((playFieldScene->width() - 150)/2, playFieldScene->height()/2 - 120);
    addWelcomeText();
    GameStarted = false;
}

void MainWidget::addWelcomeText()
{
    QGraphicsTextItem * text = playFieldScene->addText("To begin the game press \"Start\" Controlls: W - Forward, \nA - Left, S - Backward, D - Right, Shift - Shoot");
    text->setFont(QFont("BankGothic Md BT", 22));
    text->setDefaultTextColor(QColor(Qt::red));
    text->setTextWidth(550);
    QFontMetrics fm(text->font());
    text->setPos((playFieldScene->width() - 550)/2, playFieldScene->height()/2 - 100);
}
