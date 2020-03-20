#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "window_sea_fight.h"
ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    ui->textip->setPlainText(settings.value(SETTINGS_HOST,  "127.0.0.0").toString());
    ui->textport->setPlainText(settings.value(SETTINGS_PORT,  "3333").toString());
    ui->textpasswd->setPlainText(settings.value(SETTINGS_PASS,  "user123").toString());




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
       // qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
//        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
//        qDebug() <<tcpServer->serverAddress() <<QString::fromUtf8("Адресс сервера!\n");
//        qDebug() <<tcpServer->serverPort() <<QString::fromUtf8("Port сервера!\n");
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        settings.setValue(SETTINGS_HOST,ui->textip->toPlainText());
        settings.setValue(SETTINGS_PORT,ui->textport->toPlainText());
        settings.setValue(SETTINGS_PASS,ui->textpasswd->toPlainText());
        settings.sync();
       // qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void ServerWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QTime::currentTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
      //  qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void ServerWindow::newuser()
{
    if(server_status==1){
       // qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        clientSocket=tcpServer->nextPendingConnection();

         ui->textinfo->append("Ip: " +  clientSocket->peerAddress().toString());
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
    }
}

void ServerWindow::slotReadClient()
{
       if (clientSocket == 0)
              clientSocket = (QTcpSocket*)sender(); // ловлю сокет клиента
    int idusersocs=clientSocket->socketDescriptor();
    QTextStream os(clientSocket);
     os.setAutoDetectUnicode(true);

    if(MyField->myShoot) //4 step  (shooting)
        {

               MyField->myShoot=false;//отключаем зацикливание

                   QString str ="";
                   str.clear();
                      str.append(passwd).append("/")
                         .append(MyField->getField()).append("/")//  массив моего поля first array pieces[1]
                         .append("s").append("/")//MyField->myShoot
                         .append(EnemyField->getField()).append("/"); //second array pieces[3]
                     os << str
                         <<"\n"
                         << QTime::currentTime().toString() << "\n";
                     ui->textinfo->append("To client (shoot):"+str+"\n");
             return;
        }


    QString str = QString::fromUtf8(clientSocket->readAll());
    // qDebug()<<str.length()<<"str.length()!!!!!!!!!!!!!"<<str;

     QStringList pieces;
     if(str.length()>passwd.length()) // if word bigger then password
                 pieces = str.split("/");


    if(str==passwd){ // с клиента приходи пароль первый раз 1 шаг
              ui->textinfo->append("From client first connenct password: "+QTime::currentTime().toString()+"\n");
              str.clear();
              str.append(passwd).append("synch").append("/")
                 .append(MyField->getField()).append("/")//  массив моего поля
                 .append("1").append("/");//MyField->myShoot
              os << str //  2 шаг отправляю клиенту пароль  плюс моего поля

               << QTime::currentTime().toString();
              ui->textinfo->append("To Client (sync) my battle field:");
          }

    else if(pieces.size() >1 && pieces[0].startsWith(passwd) && pieces[0].endsWith("synch")) //3 step
    {
           ui->textinfo->append("From Client (synch): "+QTime::currentTime().toString()+"\n");
           str.clear();
           EnemyField->fillEnemyFieldFromConnect(pieces[1]);
           ui->textinfo->append("The end of synch "+QTime::currentTime().toString()+"\n");
         //  os<<passwd+"/"+"The end of synch"+"/"+QTime::currentTime().toString();
    }else
    if(!MyField->myShoot  && pieces.size()>3 && pieces[0]== passwd && pieces[2]== "c")//4 step  (shooting)
        {
               ui->textinfo->append("From Client (shoot):"+str+QTime::currentTime().toString()+"\n");
               EnemyField->myShoot=true;//ждем нажатия кнопки Огонь противника drawCell();

               // получили первым массивом pieces[1] поле противника
             MyField->fillMyFieldFromConnect(pieces[3]);
               //вторым массивом  мое поле измененное   pieces[3]
             EnemyField-> fillEnemyFieldFromConnect(pieces[1]);

                      os << str
                         <<"\n"
                         << QTime::currentTime().toString() << "\n";





        }else{
        os << "You not our friend!!! :"
           << QTime::currentTime().toString() << "\n";

       ui->textinfo->append("Someone stranger.... :"+str+"\n");
       clientSocket->close();
       SClients.remove(idusersocs);
   }
 //os.flush();

}

void ServerWindow::setMyFild(Field *f){
    this->MyField = f;
}
void ServerWindow::setEnemyFild(Field *field){
    this->EnemyField = field;
}

