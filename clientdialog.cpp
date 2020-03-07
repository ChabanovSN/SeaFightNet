#include "clientdialog.h"
#include "ui_clientdialog.h"
#include "window_sea_fight.h"
#include <QtGui>
#include <QDebug>


ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{



    ui->setupUi(this);
    passwd = ui->textpasswd->text();
   // clientSocket  = new  QTcpSocket(this);
    connect(ui->get, SIGNAL(clicked(bool)), this, SLOT(onSokReadyRead()));
    connect(&clientSocket,SIGNAL(connected()), this, SLOT(onSokReadyRead()));
     connect(&clientSocket,SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(ui->btnConnect,SIGNAL(clicked(bool)),this,SLOT(onSokConnected()));
   // connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
     connect(ui->btnDiconnect,SIGNAL(clicked(bool)),this,SLOT(onSokDisconnected()));

    connect(&clientSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(this, "Error", "The following error occurred: "+clientSocket.errorString());
    }
}

void ClientDialog::onSokReadyRead(){


    QTextStream os(&clientSocket);
    os.setAutoDetectUnicode(true);

    if( MyField->myShoot ){
               MyField->myShoot=false;//ждем нажатия кнопки Огонь противника
                                   //(клинет т оже начинает с MyField->myShoot=true для синхронизации

            QString str="";
            str.clear();
            str.append(passwd).append("/")
                    .append(MyField->getField()).append("/")
                    .append("1").append("/")
                    .append(EnemyField->getField()).append("/");  //second array pieces[3]

            os << str
            <<"\n"
            << QTime::currentTime().toString() << "\n";
            AddToLog("Server to (shoot):"+str+"\n"+QTime::currentTime().toString()+"\n");
           //  clientSocket.close();
       // }
             str.clear();
            return;
        }

    QString str = QString::fromUtf8(clientSocket.readAll());
     qDebug()<<str.length()<<"str.length()!!!!!!!!!!!!!"<<str;
     QStringList pieces;
     if(str.length()>passwd.length()) // if word bigger then password
                 pieces = str.split("/");

    if(str.length()==0){ // получаю первый ответ от сервера (пустая строка) 1
          AddToLog("From server  1 step empry answer :"+str+QTime::currentTime().toString()+"\n");
     os<<passwd; // отправляю пароль 2
          AddToLog("To server 1 step password :"+passwd+" "+QTime::currentTime().toString()+"\n");
    }
   else   if(pieces.size() >1 && pieces[0].startsWith(passwd) && pieces[0].endsWith("synch")){

        AddToLog("From server (synch):"+QTime::currentTime().toString()+"\n");
            str.clear();
            str.append(passwd).append("synch").append("/").append(MyField->getField()).append("/");
               EnemyField->fillEnemyFieldFromConnect(pieces[1]);
            os << str
            <<"\n"
            << QTime::currentTime().toString() << "\n";
            AddToLog("To server (synch):"+QTime::currentTime().toString()+"\n");
           //  clientSocket.close();
           str.clear();

}else if(!MyField->myShoot && EnemyField->myShoot && pieces.size()>3 && pieces[0]== passwd){


        AddToLog("Server from (shoot) :"+str+QTime::currentTime().toString()+"\n");

                // получили первым массивом pieces[1] поле противника
               MyField->fillMyFieldFromConnect(pieces[3]);
                //вторым массивом  мое поле измененное   pieces[3]
               EnemyField->fillEnemyFieldFromConnect(pieces[1]);

           //  clientSocket.close();
       // }
        }
}



void ClientDialog::onSokConnected()
{
   // on_pbConnect_clicked();
     clientSocket.connectToHost(QHostAddress(ui->textHost->text()), ui->spinPort->value());
     if(clientSocket.isOpen())
         AddToLog("Connect is open ",Qt::green);
        else
         AddToLog("Error to connect",Qt::green);

 }

void ClientDialog::onSokDisconnected()
{
    clientSocket.disconnectFromHost();
    ui->btnConnect->setEnabled(true);
    ui->btnDiconnect->setEnabled(false);
    AddToLog("Disconnected from"+clientSocket.peerAddress().toString()+":"+QString::number(clientSocket.peerPort()), Qt::green);
}

void ClientDialog::AddToLog(QString text, QColor color)
{


    ui->textinfo->append(QTime::currentTime().toString()+" "+text);
    ui->textinfo->setTextColor(color);
}
void ClientDialog::setMyFild(Field *f){
    this->MyField = f;
}
void ClientDialog::setEnemyFild(Field *field){
    this->EnemyField = field;
}
