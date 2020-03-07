#ifndef ENDWINDOW_H
#define ENDWINDOW_H

#include <QDialog>
#include <QPushButton>
namespace Ui {
class EndWindow;
}

class EndWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EndWindow(QWidget *parent = 0);

    ~EndWindow();
    void setWinner(bool);
    QPushButton *ext;
    QPushButton *restart;
private:
    Ui::EndWindow *ui;

};

#endif // ENDWINDOW_H
