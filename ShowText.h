#ifndef SHOWTEXT_H
#define SHOWTEXT_H

#include <QWidget>
#include <QList>
#include "CompilerCommon.h"
#include "CompilerCore.h"
#include "mainwindow.h"

QList<int> StringToList(QString pos);



QString ListToString(QList<QString> list);

class ShowText : public QObject
{
    Q_OBJECT

public:
    ShowText();

private:

    QList<QString> outList;

    MainWindow *w;          //加载mainwindow单例
public slots:

    //转化文本
    void transListContents(QStringList list,QString OriPos,QString Rfid);

signals:
    void sendData(QString);


};

#endif // SHOWTEXT_H
