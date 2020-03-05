#include "window_sea_fight.h"

Window::Window(QWidget *parent) : QWidget(parent)
{
    My = new MyField(300,300);
    Enemy = new EnemyField(300, 300);

    lblMyCount = new QPushButton(tr("Cells are: 0/0"), this);
    server     = new QPushButton(tr("Server"),this);
    server->setEnabled(false);
    client    = new QPushButton(tr("Client"),this);
    client->setEnabled(false);
    cleanField = new QPushButton(tr("Clean"),this);
    randomPos = new QPushButton(tr("Random position ships"),this);
    fire      = new QPushButton(tr("Fire!!!"),this);
    fire->setEnabled(false);
    playWithComp = new QPushButton(tr("Play with computer"),this);
    playWithComp->setEnabled(false);

    lblEnemyCount = new QPushButton(tr("Cells are : 0/0"), this);
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

    // TODO: Сделать, чтобы в выводе писало с какого поля был клик
    connect(My, SIGNAL(sendMouseCoord(int,int)), this, SLOT(getMouseCoord(int,int)));
    connect(Enemy, SIGNAL(sendMouseCoord(int,int)), this, SLOT(getMouseCoord(int,int)));

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

//    connect(debEn, SIGNAL(clicked(bool)), Enemy, SLOT(getField()));
//    connect(debMy, SIGNAL(clicked(bool)), My, SLOT(getField()));
      connect(randomPos,SIGNAL(clicked(bool)),My,SLOT(drawRandomPos()));
       connect(cleanField,SIGNAL(clicked(bool)),My,SLOT(clean()));
         connect(cleanField,SIGNAL(clicked(bool)),Enemy,SLOT(clean()));
         connect(cleanField,SIGNAL(clicked(bool)),this,SLOT(freeButtons()));
}

void Window::getMouseCoord(int x, int y)
{
//    My->getFieldByXY(x,y);
//    qDebug() << QString("X: %1; Y: %2").arg(QString::number(x)).arg(QString::number(y));
}

void Window::setMyCountOfCells(int myCountCells,int myCountCellsDead)
{
    Q_UNUSED(myCountCellsDead);

   // lblMyCount->setText(QString("Cells are: %1/20").arg(QString::number(myCountCells)));
     lblMyCount->setText(QString(tr("My battle field: %1/0"))
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

void Window::setEnemyCountOfCells(int enemyCountCells, int enemyCountCellsDead)
{
   Q_UNUSED(enemyCountCells);
    Q_UNUSED(enemyCountCellsDead);
    lblEnemyCount->setText(QString(tr("Enemy's battle field: 20/%1"))
                      .arg(QString::number(Enemy->getDeadShip())));
//    lblEnemyCount->setText(QString(tr("Quintity of cells: %1/20")).arg(QString::number(enemyCountCells)));
//    if (enemyCountCells == 20)
//        lblEnemyCount->setEnabled(true);
//    else
//        lblEnemyCount->setEnabled(false);
}
void Window::startServer(){
     setWindowTitle(tr("You play like SERVER"));
    My->myShoot=false;// если первый вестрел за вами
    Enemy->myShoot=false;

     qDebug() << QString("Start server");
    serverWindow = new ServerWindow();
    serverWindow->setMyFild(My);
    serverWindow->setEnemyFild(Enemy);
    client->setEnabled(false);
    playWithComp->setEnabled(false);
    randomPos->setEnabled(false);
   //  fire->setEnabled(true); // after synch
   compPlayer = 0;
    serverWindow->show();
}
void Window::startClient(){
    setWindowTitle(tr("You play like CLIENT"));
     My->myShoot=true;// если вы клиент то первый выстрел за сервером но первое тру для синхронизации
     Enemy->myShoot=true;
     qDebug() << QString("Start client");
     clientWindow = new ClientDialog();
     clientWindow ->setMyFild(My);
     clientWindow ->setEnemyFild(Enemy);
     server->setEnabled(false);
     playWithComp->setEnabled(false);
      randomPos->setEnabled(false);
    //  fire->setEnabled(true); // after synch
      compPlayer = 0;
     clientWindow->show();
}

void Window::startPlayWithComp(){
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
void Window::freeButtons(){
    server ->setEnabled(true);
    client->setEnabled(true);
    playWithComp->setEnabled(true);
     randomPos->setEnabled(true);
}
Field* Window::getFieldClass(){
    return NULL;
}

void Window::fireToEnemy(){

    if(compPlayer !=0)
        compPlayer->fireComp();
    fire->setEnabled(false);

    lblEnemyCount->setText(QString(tr("Enemy's battle field: 20/%1"))
                      .arg(QString::number(Enemy->getDeadShip())));
    lblMyCount->setText(QString(tr("My battle field: 20/%1"))
                      .arg(QString::number(My->getDeadShip())));
}

void Window::fireBtnOffOnSet(){
    fire->setEnabled(true);
}

