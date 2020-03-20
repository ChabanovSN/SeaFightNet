#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include<QSettings>
#define SETTINGS_HOST "settings/host"
#define SETTINGS_PORT "settings/port"
#define SETTINGS_PASS "settings/pass"

namespace Ui {
class ServerWindow;
}
class QTcpServer;
class Field;
class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();

private slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();


private:
    Ui::ServerWindow *ui;
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QString passwd;
    QSettings settings;
//    Field *MyField;
//    Field *EnemyField;
public slots:
     void slotReadClient();
public:
    void setMyFild(Field  *field = 0);
    void setEnemyFild(Field  *field = 0);
    Field *MyField;
    Field *EnemyField;
    QTcpSocket* clientSocket;
};

#endif // SERVERWINDOW_H
