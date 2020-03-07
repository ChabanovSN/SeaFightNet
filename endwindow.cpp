#include "endwindow.h"
#include "ui_endwindow.h"



EndWindow::EndWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EndWindow)
{
    ui->setupUi(this);



    restart = new QPushButton(QString::fromUtf8("Рестарт"),this);
    restart->setGeometry(20, 260, 100, 30);
    restart->show();
    ext = new QPushButton(QString::fromUtf8("Выход"),this);
    ext->setGeometry(280, 260, 100, 30);
    ext->show();

}




EndWindow::~EndWindow()
{
    delete ui;
}

void EndWindow::setWinner(bool b){
    QPixmap *bkgnd;
    if(b)
        bkgnd = new QPixmap(":/win.jpeg");
    else
        bkgnd = new QPixmap(":/lose.png");

    *bkgnd = bkgnd->scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Background, *bkgnd);
    this->setPalette(palette);

    delete bkgnd;
}

