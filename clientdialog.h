#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H


#include<QSettings>
#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#define SETTINGS_HOST "settings/host"
#define SETTINGS_PORT "settings/port"
#define SETTINGS_PASS "settings/pass"
namespace Ui {
class ClientDialog;
}
class QTcpSocket;
class Field;
class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();

private slots:
    //определим слоты для обработки сигналов сокета
    void onSokConnected();
    void onSokDisconnected();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);


private:
    Ui::ClientDialog *ui;
    QTcpSocket  clientSocket;
    QString passwd;
    void AddToLog(QString text, QColor color = Qt::black);
    QSettings settings;
    Field *MyField;
    Field *EnemyField;
public slots:
     void onSokReadyRead();
public:
    void setMyFild(Field  *field = 0);
    void setEnemyFild(Field  *field = 0);
};

#endif // CLIENTDIALOG_H
