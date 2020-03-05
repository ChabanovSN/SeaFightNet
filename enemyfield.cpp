#include "enemyfield.h"

EnemyField::EnemyField(int hX, int hY)
{
    pm = new QPixmap(hX, hY);
    pm->fill(Qt::gray);
   // drawNonActiveField();
    width = hX;
    hieght = hY;
    zero_x = hX / 10;
    zero_y = hY / 10;
    editingMode = false;
    drawField();
    playerName = "Enemy player";
    myField = false;

    count = 0;  // первоначально на поле 0 расставленых клеток

    // очищаем массив клеток на всякий случай
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            FIELD[i][j] = 0;

}

void EnemyField::startEditing()
{
//    QMessageBox::information(this, "Position ships", QString("Now %1 position ships").arg(playerName));
    editingMode = true;
//    pm->fill();
//    drawField();
}

void EnemyField::endEditing()
{
    editingMode = false;
//    emit startingGame();
}


