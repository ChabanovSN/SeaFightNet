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


#define SCREEN_WIDTH  1000
#define SCREEN_HIEGHT 600


class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);
    MyField *My;
    EnemyField *Enemy;
    // Не спрашивайте, почему я вместо лейблов поставил кнопки
    QPushButton *lblMyCount;     // эти надписи будут показывать
    QPushButton *lblEnemyCount;  // сколько клеток расставлено по полям
    QPushButton *debMy;
    QPushButton *debEn;
    QPushButton *server;
    QPushButton *client;
    ServerWindow *serverWindow;
    ClientDialog *clientWindow;
    QPushButton *cleanField;
    QPushButton *playWithComp;

signals:
    void sendName(const QString &name); // сигнал для отправки имени игрока
    void startEditing();    // разрешить редактирование
    void startGame();   // начать игру
//    void btStartServer();
//    void btStartClient();
//public slots:
private slots:
    void getMouseCoord(int x, int y);
    void setMyCountOfCells(int myCountCells);
    void setEnemyCountOfCells(int enemyCountCells);
    void startServer();
    void startClient();
    void startPlayWithComp();
    Field *  getFieldClass();
private:
    void freeButtons();
};

#endif // WINDOW_H
