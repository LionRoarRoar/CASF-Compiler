#include "mainwindow.h"
#include <QApplication>
#include "QIcon"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w;
    w = MainWindow::GetInstance();
    w->setWindowTitle("小猪CASF脚本编辑器");
    QString strPath = QApplication::applicationDirPath();
    strPath += "/ico/icon1.jpg";
    w->setWindowIcon(QIcon(strPath));
    w->show();

    return a.exec();
}
