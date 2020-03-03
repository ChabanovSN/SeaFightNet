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


}

//void  MyField::endEditing() override{
//    editingMode = false;

//}

//void MyField::startEditing()
//{
//    QMessageBox::information(this, "Расстановка", QString("Сначала %1 расставляет свои корабли").arg(playerName));
//}
