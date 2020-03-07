#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>
enum CELLS {CL_EMPTY = 0, CL_CELL, CL_DOT, CL_INJURED};



class Field : public QWidget
{
    Q_OBJECT
public:
    explicit Field(QWidget *parent = 0);
    int count;
    bool myField=true; // булева для выбора варианта отрисовки
    bool myShoot=true; //чей ход ходить
    int FIELD[10][10];

protected:
    virtual void paintEvent(QPaintEvent *event);
    QPixmap *pm;
    QPainter *painter;
    QPainter   *painterThis;
    QString playerName;
    bool editingMode;   // режим редактирования клеток

    int width;  // ширина поля
    int hieght;  // высота поля
    int zero_x; //  НУЛЕВЫЕ ТОЧКИ,
    int zero_y; //  с которых идет рисование поля
    int cell;   //  размер(сторона) одной клетки

    int countOfShooting=0; // это для прицела что бы был только один выстрел

    void mousePressEvent(QMouseEvent *event);
    virtual void drawField();
    virtual void drawCell(int x, int y, CELLS cellType = CL_CELL);



    /// Функции для дебага
    QString debugGetField();   // вывод значений поля
signals:
    void sendMouseCoord(int x, int y);
    void sendCountCells(int countCells,int countCellsDead );    // сигнал, передающий колличество расставленных клеток на поле
    void startEditingForEnemy();    // после того, как мы расставили свои ячейки, разрешим сделать это сопернику
    void btStartServer();
    void btStartClient();
    void fireBtnOnOff();

public slots:
    int getDeadShip();
    void clean();
    void drawRandomPos(); //рандомная отрисовка
    virtual void endEditing();  // слот для отключения режима редактирования клеток

    virtual void startEditing();    // разрешать редактирование своих ячеек
    void drawPlayField();         // рисует поле с кораблями/попаданиями и прочие мимо

    QString getField();
   // void getFieldByXY( int x, int y); // for debuger
    void fillEnemyFieldFromConnect(QString  str);
    void fillMyFieldFromConnect(QString  str);
    void drawWithoutLiveShip();


};

#endif // FIELD_H
