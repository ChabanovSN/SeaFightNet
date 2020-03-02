#include "field.h"

Field::Field(QWidget *parent) : QWidget(parent)
{
    //pm = new QPixmap();// new QPixmap(":/zero.jpg");
    //pm->scaled(50,50,Qt::KeepAspectRatio);
  //  pm->fill();

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

    p.drawPixmap(zero_x, zero_y, x, y, *pm);

}

void Field::drawField()
{
    QPainter pntr(pm);
    // нарисуем рамку
//    pntr.setPen(QPen(Qt::black, 1, Qt::SolidLine));
//    pntr.drawRect(QRect(QPoint(zero_x, zero_y), QPoint(x-2, y-2)));

    // Теперь нарисуем собственно деления/cells
    pntr.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    cell = x / 11; /// width / 10  zero_y 0 , y = hieght
    //QMessageBox::information(this, "debug", QString::number(cell));
    for (int i = 1; i < 12; i++)
    {
        pntr.drawLine(QPoint(i*cell, zero_y), QPoint(i*cell, y)); // vertical
        pntr.drawLine(QPoint(zero_x, i*cell), QPoint(x, i*cell)); // horizontal
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
    {
        if (editingMode == true)
            drawCell(event->x(), event->y(), CL_CELL);
        else
            QMessageBox::information(this, "", "Field is closed");
    }
    if (event->button() == Qt::RightButton)
        if (editingMode == true)
            drawCell(event->x(), event->y(), CL_EMPTY);
    emit sendMouseCoord(event->x(), event->y());
}

void Field::drawCell(int xP, int yP/*, CELLS cellType = CL_CELL*/, CELLS cellType)
{
    // индексы для массива клеток
    // ВНИМАНИЕ, их надо в индексах будет поменять местами
    int c_x, c_y;
    QPainter pntr(pm);

    if ((xP >=zero_x+cell && xP <= x+cell) && (yP >=zero_y+cell && yP <= y+cell))
    {
        // находим индексы для массива
        c_x = (xP - cell) / cell;
        c_y = (yP - cell) / cell;

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
            pntr.setBrush(QBrush(QColor(176,224,230)));
            pntr.setPen(QPen( QColor(176,224,230)));
            pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            if (FIELD[c_y-1][c_x-1] == 1)
            {
                FIELD[c_y-1][c_x-1] = 0;    // заполняем наш массив клеток
                count--;
                emit sendCountCells(count);
            }

        }
        if (cellType == CL_CELL)    //  1
        {
//            pntr.setBrush(QBrush(Qt::black));
//            pntr.setPen(QPen(Qt::red));
            //pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
             QPixmap  *ship = new QPixmap(":/ship.jpg");
            // ship->scaled(cell-2,cell-2,Qt::KeepAspectRatio);
             // ship->rect();
             pntr.drawPixmap(c_x*cell+1, c_y*cell+1,cell-2,cell-2,*ship);

            if (FIELD[c_y-1][c_x-1] == 0)
            {
                FIELD[c_y-1][c_x-1] = 1;
                count++;
                emit sendCountCells(count);
            }
        }
        if (cellType == CL_DOT)     //  2
        {
            pntr.setBrush(QBrush(Qt::black));
            pntr.setPen(QPen(Qt::black));
            // рисование круга небольшим костылем, т.к. я не знал,
            // как его сделать ровно по центру клетки
            pntr.drawArc(QRect(c_x*cell+cell-12, c_y*cell+cell-12, 5, 5), 0, 36000);
            FIELD[c_y-1][c_x-1] = 2;
        }
        if (cellType == CL_INJURED) //  3 - т.е раненая клетка
        {
            pntr.setBrush(QBrush(QColor(255, 0, 0, 140)));  // м.б. позже убрать альфу
            pntr.setPen(QPen(QColor(255, 0, 0, 140)));
            pntr.drawRect(QRect(c_x*cell+1, c_y*cell+1, cell-2, cell-2));
            FIELD[c_y-1][c_x-1] = 3;
        }


    }
    catch (...)
    {

    }
    update();
}

void Field::endEditing()
{
    editingMode = false;
   // clean();
   // drawNonActiveField();
//    QMessageBox::information(this, "debug", debugGetField());
    emit startEditingForEnemy();
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
    drawField();
}

void Field::startEditing() {}

void Field::drawPlayField()
{
    pm->fill();
    drawField();
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (FIELD[i][j] == 0)
                drawCell(i*cell, j*cell, CL_EMPTY);
            if (FIELD[i][j] == 1)
                drawCell(i*cell, j*cell, CL_CELL);
            if (FIELD[i][j] == 2)
                drawCell(i*cell, j*cell, CL_DOT);
            if (FIELD[i][j] == 3)
                drawCell(i*cell, j*cell, CL_INJURED);
        }
    }
    update();
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
   // QMessageBox::information(this, "debug", temp);
    return temp;
}

void Field::getFieldByXY(int x, int y){
   int c_x, c_y;
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

   qDebug() << QString("c_x: %1; c_y: %2  x= %3   y= %4    cell = %5  ")
               .arg(QString::number(c_x))
               .arg(QString::number(c_y))
               .arg(QString::number(x))
               .arg(QString::number(y))
               .arg(QString::number( cell))
               ;
}

void Field::fillEnemyFieldFromConnect(QString str){
        QStringList pieces = str.split( "," );
        int step = 0;

        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++){
               if(step<pieces.size())
                FIELD[i][j] = pieces[step++].toInt();
            }

}
