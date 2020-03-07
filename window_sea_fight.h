#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

#include "myfield.h"
#include "enemyfield.h"
#include "serverwindow.h"
#include "clientdialog.h"
#include "compasplayer.h"
#include "endwindow.h"
#define SCREEN_WIDTH  1000
#define SCREEN_HIEGHT 600


class Window_sea_fight : public QWidget
{
    Q_OBJECT
public:
    explicit Window_sea_fight(QWidget *parent = 0);
    ~Window_sea_fight();
    MyField *My = 0;
    EnemyField *Enemy = 0;
    CompAsPlayer *compPlayer = 0;
    EndWindow * endWindow = 0;
    // Не спрашивайте, почему я вместо лейблов поставил кнопки
    QPushButton *lblMyCount;     // эти надписи будут показывать
    QPushButton *lblEnemyCount;  // сколько клеток расставлено по полям
    QLabel *debMy;
    QLabel *debEn;
    QPushButton *server;
    QPushButton *client;
    ServerWindow *serverWindow = 0;
    ClientDialog *clientWindow = 0;
    QPushButton *cleanField;
    QPushButton *fire;
    QPushButton *playWithComp;
    QPushButton *randomPos;

//signals:
//    void sendName(const QString &name); // сигнал для отправки имени игрока
//    void startEditing();    // разрешить редактирование


private slots:
    void fireToEnemy();
  //  void getMouseCoord(int x, int y);
    void setMyCountOfCells(int myCountCells,int myCountCellsDead);
    void setEnemyCountOfCells(int enemyCountCells,int enemyCountCellsDead);
    void startServer();
    void startClient();
    void startPlayWithComp();

     void fireBtnOffOnSet();
     void freeButtons();
};

#endif // WINDOW_H
