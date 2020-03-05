#include "myfield.h"

MyField::MyField(int hX, int hY)
{
    pm =new QPixmap(hX, hY);

//   pm = new QPixmap(":/sea_bg.jpg");
// //  pm->scaled(hX,hY,Qt::KeepAspectRatio);

    width = hX;
    hieght = hY;
    zero_x = hX / 10;
    zero_y = hY / 10;
    myField = true;


    pm->fill(QColor(176,224,230));
    editingMode = true;
    drawField();
    playerName = " I'm player";

    count = 0;  // первоначально на поле 0 расставленых клеток

    // очищаем массив клеток на всякий случай
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            FIELD[i][j] = 0;

}

void MyField::endEditing(){
    editingMode =  !editingMode;
}

//void MyField::startEditing()
//{
//    QMessageBox::information(this, "Расстановка", QString("Сначала %1 расставляет свои корабли").arg(playerName));
//}
