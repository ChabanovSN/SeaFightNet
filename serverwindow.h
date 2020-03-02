#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>


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
    void slotReadClient();
private:
    Ui::ServerWindow *ui;
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QString passwd;
    Field *MyField;
    Field *EnemyField;
public:
    void setMyFild(Field  *field = 0);
    void setEnemyFild(Field  *field = 0);
};

#endif // SERVERWINDOW_H
