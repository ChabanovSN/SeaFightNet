#ifndef MYFIELD_H
#define MYFIELD_H

#include "field.h"

class MyField : public Field
{
    Q_OBJECT
public:
    MyField(int hX, int hY);
    int FIELD[10][10]; // собственно, поле
public slots:
    // в принципе, в слоты ставить этот метод не обязательно
//    void startEditing();    // разрешить редактирование своих клеток

};

#endif // MYFIELD_H
