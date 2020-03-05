#ifndef ENEMYFIELD_H
#define ENEMYFIELD_H

#include "field.h"

class EnemyField : public Field
{
    Q_OBJECT
public:
    EnemyField(int hX, int hY);
    int FIELD[10][10]; // собственно, поле
public slots:
    void startEditing();// слот для включения режима редактирования клеток
    void endEditing();  // закрываем доступ к редактированию и, собственно, начинаем игру

signals:
    void startingGame();

};

#endif // ENEMYFIELD_H
