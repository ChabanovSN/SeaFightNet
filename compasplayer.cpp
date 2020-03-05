#include "compasplayer.h"
#include "window_sea_fight.h"
CompAsPlayer::CompAsPlayer()
{

}
void CompAsPlayer::setMyFild(Field *f){
    this->MyField = f;
}
void CompAsPlayer::setEnemyFild(Field *field){
    this->EnemyField = field;
    this->EnemyField->drawRandomPos();
}

void CompAsPlayer::fireComp(){
    srand(time(NULL));
    int i = 0;
    int j = 0;
qDebug()<<"EnemyField->myShoot"<<EnemyField->myShoot ;

    while(EnemyField->myShoot){
        i = rand()%11;
        j = rand()%11;
        if(MyField->FIELD[i][j] == 0){
           EnemyField->myShoot=false;
           MyField->FIELD[i][j] = -10;

        } else  if(MyField->FIELD[i][j] == 1){
            MyField->FIELD[i][j] = 10; // если попали то ставим промохи в соседние поля
           if(i-1 != 0 ) MyField->FIELD[i-1][j] = -10;
           if(i+1 != 10 ) MyField->FIELD[i+1][j] = -10;
           if(j-1 != 0 ) MyField->FIELD[i][j-1] = -10;
           if(j+1 != 10 ) MyField->FIELD[i][j+1] = -10;
            EnemyField->myShoot=false;
     }
      qDebug()<<"comp player"<<MyField->FIELD[i][j] ;
    }


    MyField->drawPlayField();
    EnemyField->drawWithoutLiveShip();
    EnemyField->setEnabled(true);
    EnemyField->startEditing();
    EnemyField->myShoot = true;
}
