#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug() << "begin";
    MainWindow window;
    window.show();
    return QApplication::exec();
}
