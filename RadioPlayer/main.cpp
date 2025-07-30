#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }

    MainWindow w;
    QStringList *keys;
    w.show();
    return a.exec();
}
