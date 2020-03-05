#ifndef COMPASPLAYER_H
#define COMPASPLAYER_H
#include "field.h"

class CompAsPlayer
{
public:
    CompAsPlayer();

private:
    Field *MyField;
    Field *EnemyField;
public slots:
    void fireComp();
    void setMyFild(Field  *field = 0);
    void setEnemyFild(Field  *field = 0);
};

#endif // COMPASPLAYER_H
