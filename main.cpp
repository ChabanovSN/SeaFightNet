#include "window_sea_fight.h"
#include <QApplication>
#include <QtPlugin>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     QTranslator translator;
    QString lang = QLocale::languageToString(
        QLocale::system().language()
    ).toLower();
    qDebug( "Locale: %s", qPrintable(lang) );
    translator.load( ":/" + lang );
    a.installTranslator( &translator );
    Window w;
    w.show();

    return a.exec();
}
