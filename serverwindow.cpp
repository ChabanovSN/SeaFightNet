#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "window.h"
ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

ServerWindow::~ServerWindow()
{
    delete ui;
    server_status=0;
}
//  QHostAddress::LocalHost
void ServerWindow::on_starting_clicked()
{
    passwd =  ui->textpasswd->toPlainText();
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress(ui->textip->toPlainText()), ui->textport->toPlainText().toInt()) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        qDebug() <<tcpServer->serverAddress() <<QString::fromUtf8("Адресс сервера!\n");
        qDebug() <<tcpServer->serverPort() <<QString::fromUtf8("Port сервера!\n");
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));

        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void ServerWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void ServerWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();

         ui->textinfo->append("Ip: " +  clientSocket->peerAddress().toString());
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void ServerWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QString str = QString::fromUtf8(clientSocket->readAll());
    QStringList pieces = str.split("/");
     ui->textinfo->append("Client from:"+str+"\n");
     QTextStream os(clientSocket);
      os.setAutoDetectUnicode(true);

   if( pieces[0]== passwd){ // можно грохнуть через телнет (passwd лялялялл)
       str.clear();
       str.append(passwd).append("/").append(MyField->getField()).append("/");
    //   qDebug()<<pieces[0] <<" pieces[0]"<<  passwd ;
       if(pieces.size()>2 )
       EnemyField->fillEnemyFieldFromConnect(pieces[1]);

       os << str
          <<"\n"
          << QTime::currentTime().toString() << "\n";
      ui->textinfo->append("Client to:"+str+"\n");
   }else{
        os << "You not our friend!!! :"
           << QDateTime::currentDateTime().toString() << "\n";

       ui->textinfo->append("Someone stranger.... :"+str+"\n");
       clientSocket->close();
       SClients.remove(idusersocs);
   }
 os.flush();

}

void ServerWindow::setMyFild(Field *f){
    this->MyField = f;
}
void ServerWindow::setEnemyFild(Field *field){
    this->EnemyField = field;
}
