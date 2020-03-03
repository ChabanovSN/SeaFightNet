#include "window.h"

Window::Window(QWidget *parent) : QWidget(parent)
{
    My = new MyField(300,300);
    Enemy = new EnemyField(300, 300);

    lblMyCount = new QPushButton(tr("Cells are: 0/20"), this);
    server     = new QPushButton(tr("Server"),this);
    client    = new QPushButton(tr("Client"),this);
    cleanField = new QPushButton(tr("Clean"),this);
    randomPos = new QPushButton(tr("Random position ships"),this);
    fire      = new QPushButton(tr("Fire!!!"),this);
    playWithComp = new QPushButton(tr("Play with computer"),this);


    lblEnemyCount = new QPushButton(tr("Quintity cells: 0/20"), this);
    lblEnemyCount->setEnabled(false);
    lblMyCount->setEnabled(false);

    debEn = new  QLabel(tr("Enemy's battle field"), this);
    debEn->setGeometry(QRect(SCREEN_WIDTH/2+35 , 20, 500, 20));

    debMy = new  QLabel(tr("My battle field"), this);
       debMy->setGeometry(QRect(40, 20, 500, 20));

    QVBoxLayout *leftField = new QVBoxLayout();
    leftField->addWidget(My);
    leftField->addWidget(randomPos);
    leftField->addWidget(cleanField);
    leftField->addWidget(server);
    leftField->addWidget(client);
    leftField->addWidget(lblMyCount);

    QVBoxLayout *rightField = new QVBoxLayout();
    rightField->addWidget(Enemy);
    rightField->addWidget(fire);
    rightField->addWidget(playWithComp);
    rightField->addWidget(lblEnemyCount);


    QGridLayout *lay = new QGridLayout(this);
    lay->addLayout(leftField, 0, 0);
    lay->addLayout(rightField, 0, 1);

    this->setLayout(lay);
    this->resize(QSize(SCREEN_WIDTH, SCREEN_HIEGHT));
    this->setMinimumSize(QSize(SCREEN_WIDTH/2, SCREEN_HIEGHT/2));

    // TODO: Сделать, чтобы в выводе писало с какого поля был клик
    connect(My, SIGNAL(sendMouseCoord(int,int)), this, SLOT(getMouseCoord(int,int)));
    connect(Enemy, SIGNAL(sendMouseCoord(int,int)), this, SLOT(getMouseCoord(int,int)));
    connect(My, SIGNAL(sendCountCells(int)), this, SLOT(setMyCountOfCells(int)));
    connect(Enemy, SIGNAL(sendCountCells(int)), this, SLOT(setEnemyCountOfCells(int)));
    connect(server,SIGNAL(clicked(bool)),this,SLOT(startServer()));
     connect(client,SIGNAL(clicked(bool)),this,SLOT(startClient()));
     connect(fire,SIGNAL(clicked(bool)),this,SLOT(fireToEnemy()));
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
}

void Window::getMouseCoord(int x, int y)
{
    My->getFieldByXY(x,y);
    qDebug() << QString("X: %1; Y: %2").arg(QString::number(x)).arg(QString::number(y));
}

void Window::setMyCountOfCells(int myCountCells)
{
    lblMyCount->setText(QString("Cells are: %1/20").arg(QString::number(myCountCells)));

    if (myCountCells >= 20){
        My->setEnabled(false);
        lblMyCount->setEnabled(true);
        Enemy->setEnabled(true);
         lblMyCount->setText(QString("FIX IT (cells are: %1/20)").arg(QString::number(myCountCells)));
    }
    else{
        freeButtons();
        lblMyCount->setEnabled(false);
        My->setEnabled(true);
        Enemy->setEnabled(false);
    }
}

void Window::setEnemyCountOfCells(int enemyCountCells)
{
    Q_UNUSED(enemyCountCells)
//    lblEnemyCount->setText(QString(tr("Quintity of cells: %1/20")).arg(QString::number(enemyCountCells)));
//    if (enemyCountCells == 20)
//        lblEnemyCount->setEnabled(true);
//    else
//        lblEnemyCount->setEnabled(false);
}
void Window::startServer(){
    My->myShoot=true;// если первый вестрел за вами
    Enemy->myShoot=false;

     qDebug() << QString("Start server");
    serverWindow = new ServerWindow();
    serverWindow->setMyFild(My);
    serverWindow->setEnemyFild(Enemy);
    client->setEnabled(false);
    playWithComp->setEnabled(false);
    serverWindow->show();
}
void Window::startClient(){
     My->myShoot=false;// если вы клиент то первый выстрел за сервером
     Enemy->myShoot=true;
     qDebug() << QString("Start client");
     clientWindow = new ClientDialog();
     clientWindow ->setMyFild(My);
     clientWindow ->setEnemyFild(Enemy);
     server->setEnabled(false);
     playWithComp->setEnabled(false);
     clientWindow->show();
}

void Window::startPlayWithComp(){
    My->myShoot=true;// если первый вестрел за вами
    Enemy->myShoot=false;
      server ->setEnabled(false);
      client->setEnabled(false);
      Enemy->drawRandomPos();
}
void Window::freeButtons(){
    server ->setEnabled(true);
    client->setEnabled(true);
    playWithComp->setEnabled(true);
}
Field* Window::getFieldClass(){
    return NULL;
}

void Window::fireToEnemy(){
    qDebug()<< " Fire";
}
