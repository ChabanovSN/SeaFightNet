#include "window_sea_fight.h"
#include "endwindow.h"
Window_sea_fight::Window_sea_fight(QWidget *parent) : QWidget(parent)
{


    QPixmap bkgnd(":/sea_bg.jpg");
    bkgnd = bkgnd.scaled(SCREEN_WIDTH*2 ,SCREEN_HIEGHT*1.2);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    endWindow = new EndWindow();
    serverWindow = new ServerWindow();
    clientWindow = new ClientDialog();
    My = new MyField(300,300);
    Enemy = new EnemyField(300, 300);

    lblMyCount = new QPushButton(QString::fromUtf8("Клеток на поле: 0/0"), this);
    server     = new QPushButton(QString::fromUtf8("Вы сервер"),this);
    server->setEnabled(false);
    client    = new QPushButton(QString::fromUtf8("Вы клиент"),this);
    client->setEnabled(false);
    cleanField = new QPushButton(QString::fromUtf8("Очистить поля"),this);
    randomPos = new QPushButton(QString::fromUtf8("Случайное поле боя"),this);
    fire      = new QPushButton(QString::fromUtf8("Огонь!!!"),this);
    fire->setEnabled(false);
    playWithComp = new QPushButton(QString::fromUtf8("Играть с компьютером"),this);
    playWithComp->setEnabled(false);

    lblEnemyCount = new QPushButton(QString::fromUtf8("Клеток на поле : 0/0"), this);
    lblEnemyCount->setEnabled(false);
    lblMyCount->setEnabled(false);




    QVBoxLayout *leftField = new QVBoxLayout();
    leftField->addWidget(lblMyCount);
    leftField->addWidget(My);
    leftField->addWidget(randomPos);
    leftField->addWidget(cleanField);
    leftField->addWidget(server);
    leftField->addWidget(client);


    QVBoxLayout *rightField = new QVBoxLayout();
    rightField->addWidget(lblEnemyCount);
    rightField->addWidget(Enemy);
    rightField->addWidget(fire);
    rightField->addWidget(playWithComp);


    QGridLayout *lay = new QGridLayout(this);
    lay->addLayout(leftField, 0, 0);
    lay->addLayout(rightField, 0, 1);

    this->setLayout(lay);
    this->resize(QSize(SCREEN_WIDTH, SCREEN_HIEGHT));
    this->setMinimumSize(QSize(SCREEN_WIDTH/2, SCREEN_HIEGHT/2));



    connect(My, SIGNAL(sendCountCells(int,int)), this, SLOT(setMyCountOfCells(int,int)));
    connect(Enemy, SIGNAL(sendCountCells(int,int)), this, SLOT(setEnemyCountOfCells(int,int)));
    connect(server,SIGNAL(clicked(bool)),this,SLOT(startServer()));
    connect(client,SIGNAL(clicked(bool)),this,SLOT(startClient()));
    connect(fire,SIGNAL(clicked(bool)),this,SLOT(fireToEnemy()));


    connect(Enemy,SIGNAL(fireBtnOnOff()),this,SLOT(fireBtnOffOnSet()));
    connect(playWithComp,SIGNAL(clicked(bool)),this,SLOT(startPlayWithComp()));
    // отключение редактирования
    connect(lblMyCount, SIGNAL(clicked(bool)), My, SLOT(endEditing()));
    connect(lblEnemyCount, SIGNAL(clicked(bool)), Enemy, SLOT(endEditing()));

    connect(My, SIGNAL(startEditingForEnemy()), Enemy, SLOT(startEditing()));
    connect(Enemy, SIGNAL(startingGame()), My, SLOT(drawPlayField()));


    connect(randomPos,SIGNAL(clicked(bool)),My,SLOT(drawRandomPos()));
    connect(cleanField,SIGNAL(clicked(bool)),My,SLOT(clean()));
    connect(cleanField,SIGNAL(clicked(bool)),Enemy,SLOT(clean()));
    connect(cleanField,SIGNAL(clicked(bool)),this,SLOT(freeButtons()));


    /// exit/restart
    connect(endWindow->ext,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(endWindow->ext,SIGNAL(clicked(bool)),endWindow,SLOT(close()));
    connect(endWindow->ext,SIGNAL(clicked(bool)),serverWindow,SLOT(close()));
    connect(endWindow->ext,SIGNAL(clicked(bool)),clientWindow,SLOT(close()));
    connect(endWindow->restart,SIGNAL(clicked(bool)),My,SLOT(clean()));
    connect(endWindow->restart,SIGNAL(clicked(bool)),Enemy,SLOT(clean()));
    connect(endWindow->restart,SIGNAL(clicked(bool)),this,SLOT(freeButtons()));
    connect(endWindow->restart,SIGNAL(clicked(bool)),endWindow,SLOT(close()));
}


void Window_sea_fight::setMyCountOfCells(int myCountCells,int myCountCellsDead)
{
    Q_UNUSED(myCountCellsDead);

    // lblMyCount->setText(QString("Cells are: %1/20").arg(QString::number(myCountCells)));
    lblMyCount->setText((QString::fromUtf8("Моих короблей живых/подбитых: %1/0"))
                        .arg(QString::number(myCountCells)));

    if (myCountCells == 20){
        My->setEnabled(false);
        My->endEditing();
        freeButtons();
        playWithComp->setEnabled(true);

    }
    else{
        //freeButtons();
        lblMyCount->setEnabled(false);
        My->setEnabled(true);
        Enemy->setEnabled(false);
        server->setEnabled(false);
        client->setEnabled(false);
        playWithComp->setEnabled(false);
    }
}

void Window_sea_fight::setEnemyCountOfCells(int enemyCountCells, int enemyCountCellsDead)
{
    Q_UNUSED(enemyCountCells);
    Q_UNUSED(enemyCountCellsDead);
    lblEnemyCount->setText((QString::fromUtf8("Короблей противника живых/подбитых: 20/%1"))
                           .arg(QString::number(Enemy->getDeadShip())));

}
void Window_sea_fight::startServer(){
    serverWindowWored = true;
    setWindowTitle(QString::fromUtf8("Вы играете на стророне сервера"));
    My->myShoot=false;// если первый вестрел за вами
    Enemy->myShoot=false;
    qDebug() << QString::fromUtf8("Сервер старт");

    serverWindow->setMyFild(My);
    serverWindow->setEnemyFild(Enemy);
    client->setEnabled(false);
    playWithComp->setEnabled(false);
    randomPos->setEnabled(false);
    //  fire->setEnabled(true); // after synch
    compPlayer = 0;
    clientWindow =0;
    Enemy->startEditing();
    Enemy->setEnabled(true);
    serverWindow->show();
}
void Window_sea_fight::startClient(){
    clientWindowWorked =true;
    setWindowTitle(QString::fromUtf8("Вы играете на стороне клиента (за Вами первый ход)"));
    My->myShoot=false;// если вы клиент то первый выстрел за сервером но первое тру для синхронизации
    // Enemy->myShoot=false;
    qDebug() << QString::fromUtf8("Клиент старт");

    serverWindow = 0;
    clientWindow ->setMyFild(My);
    clientWindow ->setEnemyFild(Enemy);
    server->setEnabled(false);
    playWithComp->setEnabled(false);
    randomPos->setEnabled(false);
    //  fire->setEnabled(true); // after synch
    compPlayer = 0;

    Enemy->startEditing();
    Enemy->setEnabled(true);
    clientWindow->show();
}

void Window_sea_fight::startPlayWithComp(){
    My->myShoot=true;// если первый вестрел за вами
    Enemy->myShoot=false;
    server ->setEnabled(false);
    client->setEnabled(false);
    compPlayer = new CompAsPlayer();
    compPlayer->setMyFild(My);
    compPlayer->setEnemyFild(Enemy);
    Enemy->startEditing();
    Enemy->setEnabled(true);
    randomPos->setEnabled(false);
    playWithComp->setEnabled(false);


}
void Window_sea_fight::freeButtons(){
    server ->setEnabled(true);
    client->setEnabled(true);
    playWithComp->setEnabled(true);
    randomPos->setEnabled(true);
    clientWindowWorked =false;
    serverWindowWored = false;
}


void Window_sea_fight::fireToEnemy(){
    int myShipDead = My->getDeadShip();
    int enemyShipDead = Enemy->getDeadShip();


    lblEnemyCount->setText((QString::fromUtf8("Короблей противника живых/подбитых: 20/%1"))
                           .arg(QString::number(enemyShipDead)));
    lblMyCount->setText((QString::fromUtf8("Моих короблей живых/подбитых: 20/%1"))
                        .arg(QString::number(myShipDead)));

    if(compPlayer !=0){
        compPlayer->fireComp();
        Enemy->myShoot=true;

        if(myShipDead == 20 || enemyShipDead == 20){
            endWindow->setWinner((myShipDead < enemyShipDead));
            endWindow->show();
        }
    }


    if(serverWindowWored){
        My->myShoot = true;
        Enemy->myShoot=false;
        serverWindow->slotReadClient();

        if(myShipDead == 20 || enemyShipDead == 20){
            endWindow->setWinner((myShipDead < enemyShipDead));
            endWindow->show();
        }
    }

    if(clientWindowWorked){
        My->myShoot = true;
        Enemy->myShoot=false;
        clientWindow->onSokReadyRead();
        if(myShipDead == 20 || enemyShipDead == 20){
            endWindow->setWinner((myShipDead < enemyShipDead));
            endWindow->show();
        }
    }

    fire->setEnabled(false);
}

void Window_sea_fight::fireBtnOffOnSet(){
    fire->setEnabled(true);
}

Window_sea_fight::~Window_sea_fight(){
    delete clientWindow;
    delete serverWindow;
    delete endWindow;


}
//debug
//void Window_sea_fight::getMouseCoord(int x, int y)
//{
//    //    My->getFieldByXY(x,y);
//    //    qDebug() << QString("X: %1; Y: %2").arg(QString::number(x)).arg(QString::number(y));
//}
