#include "field.h"


Field::Field(QWidget *parent) : QWidget(parent)
{

    count = 0;  // первоначально на поле 0 расставленых клеток
    // очищаем массив клеток на всякий случай
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            FIELD[i][j] = 0;


}

void Field::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    painterThis = new QPainter(this);
    painterThis->drawPixmap(zero_x, zero_y, width, hieght, *pm);
    // update();
    delete painterThis;

}

void Field::drawField(){    

    // рисуем сетку
    painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    cell = width / 11; /// width / 10  zero_y 0 , y = hieght

    for (int i = 1; i < 12; i++)
    {
        painter->drawLine(QPoint(i*cell, zero_y), QPoint(i*cell, hieght)); // vertical
        painter->drawLine(QPoint(zero_x, i*cell), QPoint(width, i*cell)); // horizontal
    }

    QChar temp[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    for (int i = 0; i < 10; i++)
    {
        painter->drawText(QRect(cell+i*cell+cell/2-2, 4, cell*2 + i*cell, cell-4), QString(temp[i]));
    }
    for (int i = 1; i < 11; i++)
    {
        painter->drawText(QRect(4, cell*i+cell/2-7, cell-4, cell), QString::number(i));
    }

    update();


}

void Field::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (editingMode == true){
            drawCell(event->x(), event->y(), CL_CELL);
        }
        else
            QMessageBox::information(this, "", "Field is closed");
    }
    else if (event->button() == Qt::RightButton){
        if (editingMode == true)
            drawCell(event->x(), event->y(), CL_EMPTY);
        else
            QMessageBox::information(this, "", "Field is closed");
    }

    emit sendMouseCoord(event->x(), event->y());
}

void Field::drawCell(int xP, int yP, CELLS cellType)
{
    // индексы для массива клеток
    int c_x, c_y;
    if ((xP >=zero_x+cell && xP <= width+cell) && (yP >=zero_y+cell && yP <= hieght+cell)) {
        // находим индексы для массива
        c_x = (xP - cell) / cell;
        c_y = (yP - cell-4) / cell;

        if(c_x>10 || c_y>10){
            c_x = -1;
            c_y = -1;
            return;
        }
    }
    else
    {
        c_x = -1;
        c_y = -1;
        return;
    }


    try
    {
        if (cellType == CL_EMPTY){
            if(!myField && countOfShooting>1){
                countOfShooting--;
                painter->setBrush(QBrush(QColor(176,224,230)));
                painter->setPen(QPen( QColor(176,224,230)));
                painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            }
            painter->setBrush(QBrush(QColor(176,224,230)));
            painter->setPen(QPen( QColor(176,224,230)));
            painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));


            FIELD[c_x-1][c_y-1] = 0;    // заполняем наш массив клеток
            if(count <20){
                count--;
                if(count<0){
                    count=0;
                    emit sendCountCells(count,0);

                }
                else
                    emit sendCountCells(count,0);

            }

        }
        if (cellType == CL_CELL){
            if(myField){
                int i = c_x-1, j = c_y-1;
                if(editingMode == true){
                    if (FIELD[i-1][j]==1 || FIELD[i+1][j]==1 || FIELD[i][j+1]==1 || FIELD[i][j-1]==1){
                        QMessageBox::information(this, tr("Atantion!"),tr("You need more space!"));
                        return;
                    }
                }

                QPixmap  *ship = new QPixmap(":/ship.jpg");
                painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*ship);

                delete ship;
                FIELD[c_x-1][c_y-1] = 1;

            }else{

                if(countOfShooting<1){
                    QPixmap  *target = new QPixmap(":/target.jpg"); // отросовка прицела
                    painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*target);


                    delete target;
                    if( FIELD[c_x-1][c_y-1]==0)
                        FIELD[c_x-1][c_y-1]=-10;
                    if( FIELD[c_x-1][c_y-1]==1){
                        int i = c_x-1, j = c_y-1;
                        FIELD[i][j]= 10;

                        if(i != 0 )  FIELD[i-1][j] = -10;
                        if(i != 9 ) FIELD[i+1][j] = -10;
                        if(j != 0 )  FIELD[i][j-1] = -10;
                        if(j != 9 ) FIELD[i][j+1] = -10;

                    }
                    countOfShooting++;
                    if(myShoot)// сработает если только была сихронизаци только для сетки
                        fireBtnOnOff();
                    emit sendCountCells(count,0);
                }else{
                    countOfShooting--;
                    painter->setBrush(QBrush(QColor(176,224,230)));
                    painter->setPen(QPen( QColor(176,224,230)));
                    painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
                    QMessageBox::information(this, tr("Atantion!"),tr("Only 1 shoot!"));
                }
            }

            if(count <20){
                count++;
                emit sendCountCells(count,0);
            }

        }
        if (cellType == CL_DOT)     //  мимо целиы
        {
            painter->setBrush(QBrush(QColor(0,0,255)));
            painter->setPen(QPen(QColor(176,224,230)));
            painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            //  repaint();

        }
        if (cellType == CL_INJURED) //  раненая клетка
        {   QPixmap  *burn3 = new QPixmap(":/burns/burn3.jpg");
            // отросовка взрыва
            painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*burn3);
            //   repaint();

            delete burn3;

        }


    }
    catch (...)
    {
        qDebug()<<"Error in prosseing drawing";
    }

    update();
}

void Field::endEditing()
{

}

void Field::clean()
{    count = 0;

     editingMode =true;
      pm->fill(QColor(176,224,230));
       for (int i = 0; i < 10; i++)
           for (int j = 0; j < 10; j++)
               FIELD[i][j] = 0;
        emit sendCountCells(count,0);
        drawField();
}



QString Field::debugGetField()
{
    QString temp;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            temp.append(QString::number(FIELD[i][j]));
        }
        temp.append("\n");
    }
    return temp;
}



void Field::startEditing() {}

void Field::drawPlayField()
{

    pm->fill(QColor(176,224,230));
    drawField();


    for (int i = 0; i < 10; i++)
    {                                    // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
        for (int j = 0; j < 10; j++)
        {

            if (FIELD[j][i] == 0) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_EMPTY);
            if (FIELD[j][i] == 1){ // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_CELL);
            }

            if (FIELD[j][i] == -10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_DOT);
            if (FIELD[j][i] == 10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5,CL_INJURED);
        }
    }


}

QString Field::getField()
{
    QString temp, str;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            temp.append(QString::number(FIELD[i][j])).append(",");
            str.append(QString::number(FIELD[j][i])).append(",");
        }
        //  qDebug()<<str;
        str.clear();
    }
    int pos = temp.lastIndexOf(QChar(','));
    temp =temp.left(pos);
    return temp;
}



void Field::fillEnemyFieldFromConnect(QString str){
    QStringList pieces = str.split( "," );
    int step = 0;
    //  QString  str1 = "";
    //  qDebug()<<"fillEnemyFieldFromConnect";
    if(pieces.size() == 100){
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++){
                FIELD[i][j] = pieces[step++].toInt();
                //  str1.append(QString::number(FIELD[j][i])).append(",");
            }
            //   qDebug()<<str1;
            //  str1.clear();
        }
    }
    drawWithoutLiveShip();

}

void Field::fillMyFieldFromConnect(QString str){
    QStringList pieces = str.split( "," );
    int step = 0;
    QString  str1="";
    // qDebug()<<"fillMyFieldFromConnect";
    if(pieces.size() == 100){
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++){
                FIELD[i][j] = pieces[step++].toInt();
                //  str1.append(QString::number(FIELD[j][i])).append(",");
            }
            // qDebug()<<str1;
            // str1.clear();
        }
    }
    drawPlayField();

}
void Field::drawRandomPos(){
    int ship, count1 =0;
    clean();
    srand(time(NULL));
    while(count1 < 20){

        for (int i = 0; i < 10; i++){
            int j = rand()%10;
            for (; j < 10; j++)
                if(count1 >=20)
                    break;
                else if (FIELD[i][j-1]==1 || FIELD[i-1][j]==1 || FIELD[i+1][j]==1 || FIELD[i][j+1]==1 )
                    continue;
                else{
                    ship = rand()%2;
                    if(ship>0 && FIELD[i][j] ==0 ){
                        count1++;
                        FIELD[i][j] = ship;
                    }

                }

        }

    }
    count =0;

    if(myField)
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                if(FIELD[j][i] ==1)
                    drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_CELL);




}

void Field::drawWithoutLiveShip(){
    countOfShooting = 0;
    pm->fill(QColor(176,224,230));
    drawField();
    for (int i = 0; i < 10; i++)
    {                                    // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
        for (int j = 0; j < 10; j++)
        {

            if (FIELD[j][i] == 0) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_EMPTY);

            if (FIELD[j][i] == -10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_DOT);
            if (FIELD[j][i] == 10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5,CL_INJURED);
        }
    }

}
int Field::getDeadShip(){
    int deadShip=0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if(FIELD[j][i] == 10)deadShip++;
    return deadShip;
}

//// debuger
//void Field::getFieldByXY(int x, int y){
    //    int c_x,c_y;
    //    if ((x >zero_x) && (y >zero_y))
    //    {
    //        // находим индексы для массива
    //        c_x = (x - cell) / cell;
    //        c_y = (y - cell) / cell;
    //        x=y=0;
    //    }
    //    else
    //    {
    //        c_x = -1;
    //        c_y = -1;
    //        return;
    //    }

    //   qDebug() << QString("c_x: %1; c_y: %2  x= %3   y= %4    cell = %5  ")
    //               .arg(QString::number(c_x))
    //               .arg(QString::number(c_y))
    //               .arg(QString::number(x))
    //               .arg(QString::number(y))
    //               .arg(QString::number( cell))
    //               ;
//}
