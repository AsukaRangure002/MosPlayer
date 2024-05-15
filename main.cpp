#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 应用信息
    QApplication::setApplicationName("MosPlayer");      // 应用名
    QApplication::setOrganizationName("Asuka");         // 组织名
    QGuiApplication::setApplicationDisplayName(QApplication::applicationName());
    QApplication::setApplicationVersion(QT_VERSION_STR);


    MainWindow w;

    // 读取样式表
    QFile qssFile(":/qss/mainStyle.qss");
    if(qssFile.open(QFile::ReadOnly)){
        w.setStyleSheet(qssFile.readAll());
    }
    qssFile.close();
    w.show();
    return a.exec();
}
