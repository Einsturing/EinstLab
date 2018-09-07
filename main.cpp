#include "widget.h"
#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle(QObject::tr("EinstLab"));
    w.show();

    return a.exec();
}
