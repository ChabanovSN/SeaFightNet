#include "enemyfield.h"

EnemyField::EnemyField(int hX, int hY)
{
    pm = new QPixmap(hX, hY);
    pm->fill(Qt::gray);

     painter = new QPainter(pm);

    width = hX;
    hieght = hY;
    zero_x = hX / 10;
    zero_y = hY / 10;
    editingMode = false;
    drawField();
    playerName = "Enemy player";
    myField = false;

    count = 0;  // первоначально на поле 0 расставленых клеток



}

void EnemyField::startEditing(){

    editingMode = true;

}

void EnemyField::endEditing()
{
    editingMode = false;
}


