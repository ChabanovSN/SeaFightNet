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

void Field::drawField()
{
    //QPainter   *painter = painter;
    // painter = new QPainter(pm);
    // нарисуем рамку
    //    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    //    painter.drawRect(QRect(QPoint(zero_x, zero_y), QPoint(x-2, y-2)));

    // Теперь нарисуем собственно деления/cells
    painter->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    cell = width / 11; /// width / 10  zero_y 0 , y = hieght
    //QMessageBox::information(this, "debug", QString::number(cell));
    for (int i = 1; i < 12; i++)
    {
        painter->drawLine(QPoint(i*cell, zero_y), QPoint(i*cell, hieght)); // vertical
        painter->drawLine(QPoint(zero_x, i*cell), QPoint(width, i*cell)); // horizontal
    }
    // painter.setFont(QFont("Times New Roman", 12, 20));
    QChar temp[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    for (int i = 0; i < 10; i++)
    {   //QMessageBox::information(this, "debug", QString(temp[i]));
        painter->drawText(QRect(cell+i*cell+cell/2-2, 4, cell*2 + i*cell, cell-4), QString(temp[i]));
    }
    for (int i = 1; i < 11; i++)
    {
        painter->drawText(QRect(4, cell*i+cell/2-7, cell-4, cell), QString::number(i));
    }

    //drawLabels();
  // while(!painter->end());
     update();
   // delete painter;

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
    // qDebug()<<" cellX -Y "<<xP<<" "<<yP<< " cellType "<<cellType;
    int c_x, c_y;
  //  QPainter *painter = new QPainter(pm);

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
        { // painter = new QPainter(pm);

            if(!myField && countOfShooting>1){
                countOfShooting--;
                painter->setBrush(QBrush(QColor(176,224,230)));
                painter->setPen(QPen( QColor(176,224,230)));
                painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            }


            painter->setBrush(QBrush(QColor(176,224,230)));
            painter->setPen(QPen( QColor(176,224,230)));
            painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));

            //           if (FIELD[c_y-1][c_x-1] == 1)
            //            {
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
            //            }

           // repaint();
          //  delete painter;
        }
        if (cellType == CL_CELL)    //  1
        {
           //  painter = new QPainter(pm);
            if(myField){
                int i = c_x-1, j = c_y-1;
              if(editingMode == true){//TO checking
                if (FIELD[i-1][j]==1 || FIELD[i+1][j]==1 || FIELD[i][j+1]==1 || FIELD[i][j-1]==1){
                    QMessageBox::information(this, tr("Atantion!"),tr("You need more space!"));
                    return;
                }
              }

                QPixmap  *ship = new QPixmap(":/ship.jpg");
                painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*ship);
                repaint();

                delete ship;
                FIELD[c_x-1][c_y-1] = 1;

            }else{

                if(countOfShooting<1){

                       QPixmap  *target = new QPixmap(":/target.jpg"); // отросовка прицела

                    painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*target);
                    repaint();

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


          //   delete painter;
        }
        if (cellType == CL_DOT)     //  мимо целиы
        { // painter = new QPainter(pm);
            painter->setBrush(QBrush(QColor(0,0,255)));
            painter->setPen(QPen(QColor(176,224,230)));
            painter->drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            repaint();
          //  delete painter;

        }
        if (cellType == CL_INJURED) //  3 - т.е раненая клетка
        { //painter = new QPainter(pm);

               QPixmap  *burn3 = new QPixmap(":/burns/burn3.jpg");
          // отросовка взрыва
            painter->drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*burn3);
            repaint();

            delete burn3;
           // delete painter;
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
        emit sendCountCells(count,0);
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
    QString temp, str;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            temp.append(QString::number(FIELD[i][j])).append(",");
            str.append(QString::number(FIELD[j][i])).append(",");
        }
          qDebug()<<str;
          str.clear();
    }
    int pos = temp.lastIndexOf(QChar(','));
    temp =temp.left(pos);
//    // QMessageBox::information(this, "debug", temp);
//    qDebug()<<"getField()"<<playerName;
//    qDebug()<<temp;
//    qDebug()<<"End of  getField() "<<playerName;
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

    while(count1 < 20){

        for (int i = 0; i < 10; i++){
            int j = rand()%10;
            for (; j < 10; j++)
                if(count1 >20)
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

void Field::checkEnemyBattleField(int i, int j){
    //           if(FIELD[j][i] == 0)
    //              drawCell((j+1)*cell+cell+5, (i+1)*cell+cell+5, CL_DOT);

}
