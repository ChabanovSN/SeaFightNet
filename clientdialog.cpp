#include "clientdialog.h"
#include "ui_clientdialog.h"
#include "window.h"
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

//    QString str ="";
//        for (int i = 0; i < 10; i++){
//            for (int j = 0; j < 10; j++)
//               str.append(QString::number(FIELD[j][i])).append(" ");
//           qDebug()<<str;
//           str="";
//        }


    QString str = QString::fromUtf8(clientSocket.readAll());
    QStringList pieces = str.split("/");
     qDebug()<<pieces.size()<<"SIZE!!!!!!!!!!!!!"<<str;
    AddToLog("Server from :"+str+"\n");
    QTextStream os(&clientSocket);
    os.setAutoDetectUnicode(true);
    // qDebug()<<pieces[0] <<" pieces[0]"<<  passwd<<  clientSocket.isOpen();
  //  if( pieces[0] != passwd){
        // EnemyField->fillEnemyFieldFromConnect(pieces[1]);
        str.clear();
        str.append(passwd).append("/").append(MyField->getField()).append("/");
         if(pieces.size()>2 )
        EnemyField->fillEnemyFieldFromConnect(pieces[1]);
        os << str
        <<"\n"
        << QTime::currentTime().toString() << "\n";
         AddToLog("Server to:"+str+"\n");
       //  clientSocket.close();
   // }
}



void ClientDialog::onSokConnected()
{
   // on_pbConnect_clicked();
     clientSocket.connectToHost(QHostAddress(ui->textHost->text()), ui->spinPort->value());

//     QString str = QString::fromUtf8(clientSocket->readAll());
//     //QStringList pieces = str.split(" ");
//     AddToLog("Client from :"+str);


//     QString str = QString::fromUtf8(clientSocket->readAll());
//     QStringList pieces = str.split( " " );
// qDebug()<<pieces[0] <<" pieces[0]"<<  passwd ;
//    if(pieces[0]== passwd){
//        qDebug()<<pieces[0] <<" pieces[0]"<<  passwd ;
//     qDebug() << QString::fromUtf8("Client starting !") <<clientSocket->isOpen()<<" open"<<clientSocket->isValid();
//     ui->btnConnect->setEnabled(false);
//     ui->btnDiconnect->setEnabled(true);
//     QTextStream os(clientSocket);

//     os.setAutoDetectUnicode(true);
//     os << QString::fromUtf8("Sea fight Client FUCK\n");

//     AddToLog("Connected to"+clientSocket->peerAddress().toString()+":"+QString::number(clientSocket->peerPort()),Qt::green);
//   }else
     if(clientSocket.isOpen())
         AddToLog("Connected to"+clientSocket.peerAddress().toString()+":"+QString::number(clientSocket.peerPort()),Qt::green);
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

//void ClientDialog::on_pbConnect_clicked()
//{
//    clientSocket->connectToHost(ui->textHost->text(), ui->spinPort->value());
//}

//void ClientDialog::on_pbDisconnect_clicked()
//{
//    clientSocket->disconnectFromHost();
//}



//void ClientDialog::on_pbSend_clicked()
//{
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out << (quint16)0;
//    if (ui->cbToAll->isChecked())
//        out << (quint8)MyClient::comMessageToAll;
//    else
//    {
//        out << (quint8)MyClient::comMessageToUsers;
//        QString s;
//        foreach (QListWidgetItem *i, ui->lwUsers->selectedItems())
//            s += i->text()+",";
//        s.remove(s.length()-1, 1);
//        out << s;
//    }

//    out << ui->pteMessage->document()->toPlainText();
//    out.device()->seek(0);
//    out << (quint16)(block.size() - sizeof(quint16));
//    _sok->write(block);
//    ui->pteMessage->clear();
//}

void ClientDialog::AddToLog(QString text, QColor color)
{
    Q_UNUSED(color);

    ui->textinfo->append(QTime::currentTime().toString()+" "+text);
}
void ClientDialog::setMyFild(Field *f){
    this->MyField = f;
}
void ClientDialog::setEnemyFild(Field *field){
    this->EnemyField = field;
}
