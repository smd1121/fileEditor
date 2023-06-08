#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qDebug() << "begin";
    QApplication::setWindowIcon(QIcon{":/icons/logo.png"});
    MainWindow window;
    window.showMaximized();
    window.show();
    return QApplication::exec();
}
