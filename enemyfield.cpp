#include "enemyfield.h"

EnemyField::EnemyField(int hX, int hY)
{
    pm = new QPixmap(hX, hY);
    pm->fill();
    drawNonActiveField();
    x = hX;
    y = hY;
    zero_x = hX / 10;
    zero_y = hY / 10;
    editingMode = false;
    drawField();
    playerName = "Player 2";
}

void EnemyField::startEditing()
{
    QMessageBox::information(this, "Position ships", QString("Now %1 position ships").arg(playerName));
    editingMode = true;
    pm->fill();
    drawField();
}

void EnemyField::endEditing()
{
    editingMode = false;
    emit startingGame();
}


