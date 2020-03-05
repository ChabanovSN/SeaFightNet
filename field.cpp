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
    QPainter p(this);

    p.drawPixmap(zero_x, zero_y, width, hieght, *pm);

}

void Field::drawField()
{
    QPainter pntr(pm);
    // нарисуем рамку
    //    pntr.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    //    pntr.drawRect(QRect(QPoint(zero_x, zero_y), QPoint(x-2, y-2)));

    // Теперь нарисуем собственно деления/cells
    pntr.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    cell = width / 11; /// width / 10  zero_y 0 , y = hieght
    //QMessageBox::information(this, "debug", QString::number(cell));
    for (int i = 1; i < 12; i++)
    {
        pntr.drawLine(QPoint(i*cell, zero_y), QPoint(i*cell, hieght)); // vertical
        pntr.drawLine(QPoint(zero_x, i*cell), QPoint(width, i*cell)); // horizontal
    }
    // pntr.setFont(QFont("Times New Roman", 12, 20));
    QChar temp[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    for (int i = 0; i < 10; i++)
    {   //QMessageBox::information(this, "debug", QString(temp[i]));
        pntr.drawText(QRect(cell+i*cell+cell/2-2, 4, cell*2 + i*cell, cell-4), QString(temp[i]));
    }
    for (int i = 1; i < 11; i++)
    {
        pntr.drawText(QRect(4, cell*i+cell/2-7, cell-4, cell), QString::number(i));
    }
    update();
    //drawLabels();


}

void Field::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {      qDebug()<<" aim  editingMode "<<editingMode << " player "<<playerName;
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
    // qDebug()<<" cellX -Y "<<xP<<" "<<yP<< " cellType "<<cellType;
    int c_x, c_y;
    QPainter pntr(pm);

    if ((xP >=zero_x+cell && xP <= width+cell) && (yP >=zero_y+cell && yP <= hieght+cell))
    {
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

    //QMessageBox::information(this, "cell",QString::number(c_x)+" - "+QString::number(c_y));
    try
    {
        if (cellType == CL_EMPTY)   //  0
        {

            if(!myField && countOfShooting>1){
                countOfShooting--;
                pntr.setBrush(QBrush(QColor(176,224,230)));
                pntr.setPen(QPen( QColor(176,224,230)));
                pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            }

            count--;
            if(count<0){
                count=0;
                emit sendCountCells(count);

            }


            pntr.setBrush(QBrush(QColor(176,224,230)));
            pntr.setPen(QPen( QColor(176,224,230)));
            pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));

            //           if (FIELD[c_y-1][c_x-1] == 1)
            //            {
            FIELD[c_x-1][c_y-1] = 0;    // заполняем наш массив клеток

            emit sendCountCells(count);


            //            }

        }
        if (cellType == CL_CELL)    //  1
        {

            if(myField){


                if (FIELD[c_x-1][c_y-2]==1 || FIELD[c_x-2][c_y-1]==1 || FIELD[c_x][c_y-1]==1 || FIELD[c_x-1][c_y]==1){
                    QMessageBox::information(this, tr("Atantion!"),tr("You need more space!"));
                    return;
                }
                QPixmap  ship(":/ship.jpg");
                pntr.drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,ship);
                FIELD[c_x-1][c_y-1] = 1;
                emit sendCountCells(count); // TODO убрать у противника!! MD!!! в коннектах
            }else{
                qDebug()<<" aim  E";
                if(countOfShooting<1){
                    QPixmap  target(":/target.jpg"); // отросовка прицела
                    pntr.drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,target);
                    if( FIELD[c_x-1][c_y-1]==0)
                        FIELD[c_x-1][c_y-1]=-10;
                    if( FIELD[c_x-1][c_y-1]==1)
                        FIELD[c_x-1][c_y-1]= 10;
                     countOfShooting++;
                }else{
                    countOfShooting--;
                    pntr.setBrush(QBrush(QColor(176,224,230)));
                    pntr.setPen(QPen( QColor(176,224,230)));
                    pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
                    QMessageBox::information(this, tr("Atantion!"),tr("Only 1 shoot!"));
                }


            }

            count++;
                emit sendCountCells(count);


        }
        if (cellType == CL_DOT)     //  мимо целиы
        {
            pntr.setBrush(QBrush(QColor(0,0,255)));
            pntr.setPen(QPen(QColor(176,224,230)));
            // рисование круга небольшим костылем, т.к. я не знал,
            // как его сделать ровно по центру клетки
            pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            // FIELD[c_x-1][c_y-1] = 2;
        }
        if (cellType == CL_INJURED) //  3 - т.е раненая клетка
        {
            QPixmap  burn3(":/burns/burn3.jpg"); // отросовка взрыва
            pntr.drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,burn3);

        }


    }
    catch (...)
    {

    }
    update();

}

void Field::endEditing()
{
////    editingMode = false;
////    // clean();
////    // drawNonActiveField();
////    //    QMessageBox::information(this, "debug", debugGetField());
////    emit startEditingForEnemy();
}

void Field::clean()
{    count = 0;
     editingMode =true;
      pm->fill(QColor(176,224,230));
       for (int i = 0; i < 10; i++)
           for (int j = 0; j < 10; j++)
               FIELD[i][j] = 0;
        emit sendCountCells(count);
        drawField();
}

void Field::setName(const QString &name)
{
    if (!name.isEmpty())
        playerName = name;
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

void Field::drawNonActiveField()
{
    pm->fill(Qt::lightGray);
//    drawField();
}

void Field::startEditing() {}

void Field::drawPlayField()
{  /*int ship = 0;*/

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
                //                qDebug()<< " ship "<<ship <<" i "<<i<<" j "<<j <<" field[][]"<<FIELD[i][j];
            }

            if (FIELD[j][i] == -10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_DOT);
            if (FIELD[j][i] == 10) // СМЕНА ИНДЕКСОВ!!!!!!!!!!!!!
                drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5,CL_INJURED);
        }
    }

    //   update();
}

QString Field::getField()
{
    QString temp;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            temp.append(QString::number(FIELD[i][j])).append(",");
        }

    }
    int pos = temp.lastIndexOf(QChar(','));
    temp =temp.left(pos);
    // QMessageBox::information(this, "debug", temp);
    qDebug()<<"getField()"<<playerName;
    qDebug()<<temp;
    qDebug()<<"End of  getField() "<<playerName;
    return temp;
}

// debuger
void Field::getFieldByXY(int x, int y){
    int c_x,c_y;
    if ((x >zero_x) && (y >zero_y))
    {
        // находим индексы для массива
        c_x = (x - cell) / cell;
        c_y = (y - cell) / cell;
        x=y=0;
    }
    else
    {
        c_x = -1;
        c_y = -1;
        return;
    }

    //   qDebug() << QString("c_x: %1; c_y: %2  x= %3   y= %4    cell = %5  ")
    //               .arg(QString::number(c_x))
    //               .arg(QString::number(c_y))
    //               .arg(QString::number(x))
    //               .arg(QString::number(y))
    //               .arg(QString::number( cell))
    //               ;
}

void Field::fillEnemyFieldFromConnect(QString str){
    QStringList pieces = str.split( "," );
    int step = 0;

    if(pieces.size() == 100){
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++){
                FIELD[i][j] = pieces[step++].toInt();
            }
    }

}
void Field::drawRandomPos(){

    int ship, count1 =0;

    clean();
    srand(time(NULL));


    while(count1 < 19){

        for (int i = 0; i < 10; i++){
            int j = rand()%11;
            for (; j < 10; j++)
                if(count1 >19)
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

void Field::checkEnemyBattleField(int i, int j){
    //           if(FIELD[j][i] == 0)
    //              drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_DOT);

}
