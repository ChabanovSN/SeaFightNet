#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "window_sea_fight.h"
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
    QTcpSocket* clientSocket = (QTcpSocket*)sender(); // ловлю сокет клиента
    int idusersocs=clientSocket->socketDescriptor();
    QString str = QString::fromUtf8(clientSocket->readAll());
     qDebug()<<str.length()<<"str.length()!!!!!!!!!!!!!"<<str;

     QStringList pieces;
     if(str.length()>passwd.length()) // if word bigger then password
                 pieces = str.split("/");

    QTextStream os(clientSocket);
     os.setAutoDetectUnicode(true);
    if(str==passwd){ // с клиента приходи пароль первый раз 1 шаг
              ui->textinfo->append("From client first connenct password: "+str+" "+QTime::currentTime().toString()+"\n");
              str.clear();
              str.append(passwd).append("synch").append("/")
                 .append(MyField->getField()).append("/")//  массив моего поля
                 .append("1").append("/");//MyField->myShoot
              os << str //  2 шаг отправляю клиенту пароль  плюс моего поля
                 <<"\n"
                 << QTime::currentTime().toString() << "\n";
              ui->textinfo->append("To Client (sync) my battle field:"+str+" "+QTime::currentTime().toString()+"\n");
          }

    else if(pieces.size()>2 && pieces[0].startsWith(passwd) && pieces[0].endsWith("synch")) //3 step
    {
           ui->textinfo->append("From Client (synch): "+str+" "+QTime::currentTime().toString()+"\n");


            //  if( pieces[0]== passwd){ // можно грохнуть через телнет (passwd лялялялл)
                  str.clear();
//                  str.append(passwd).append("synch").append("/")
//                     .append(MyField->getField()).append("/")//  массив моего поля
//                     .append("1").append("/");//MyField->myShoot

//                      if(pieces.size()>2 )
                         EnemyField->fillEnemyFieldFromConnect(pieces[1]);

//                  os << str
//                     <<"\n"
//                     << QTime::currentTime().toString() << "\n";
                 ui->textinfo->append("The end of synch "+QTime::currentTime().toString()+"\n");
            //  }



     }else if(MyField->myShoot && pieces.size()>3 && pieces[0]== passwd) //4 step  (shooting)
    {
           ui->textinfo->append("From Client (shoot):"+str+QTime::currentTime().toString()+"\n");
           MyField->myShoot=false;//ждем нажатия кнопки Огонь противника

                  str.clear();
                  str.append(passwd).append("/")
                     .append(MyField->getField()).append("/")//  массив моего поля
                     .append("1").append("/");//MyField->myShoot

                      if(pieces.size()>2 )
                         EnemyField->fillEnemyFieldFromConnect(pieces[1]);

                  os << str
                     <<"\n"
                     << QTime::currentTime().toString() << "\n";
                 ui->textinfo->append("To client (shoot):"+str+"\n");




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

