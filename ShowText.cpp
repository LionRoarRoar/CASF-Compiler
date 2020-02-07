#include "ShowText.h"
#include <QDebug>
#include <QString>


#define CMD_STA 1
#define CMD_END 2
#define CMD_MOV 3
#define CMD_DEY 4
#define CMD_CMG 5
#define CMD_JMP 6




QMap<QString,int> map_StrcmdToCode =
{
    {"STA",1},
    {"END",2},
    {"MOV",3},
    {"DEY",4},
    {"CMG",5},
    {"JMP",6}
};



QList<int> StringToList(QString pos)
{
    QList<int> carrierPos;
    carrierPos.clear();
    QString param ;
    QStringList Pos =pos.split(" ");
    for(int index=0;index<Pos.size();index++)
    {
        param=Pos.at(index);
        carrierPos<<param.toInt();
    }

    return carrierPos;
}

QString ListToString (QList<QString> list)
{
    QString str;
    for(int i=0; i<list.count();i++)
    {
        str.append(list.at(i)+"\r\n");
    }
    return str;
}

ShowText::ShowText()
{

}


void ShowText::transListContents(QStringList list,QString OriPos,QString Rfid)
{
    if (list.isEmpty() || OriPos.isEmpty() || Rfid.isEmpty()) return;

    //把string型OriPos 转换为 int型list的CarrierPos
    QList<int>CarrierPos ;
    QStringList OriPosList = OriPos.split(" ");
    for(int i = 0; i<OriPosList.size();i++)
    {
        QString pos =  OriPosList.at(i);
        CarrierPos.append(pos.toInt());
    }

    //把string型的Rfid转为int型的rfidNum
    int rfidNum = Rfid.toInt();

    qDebug()<<"CarrierPos"<<CarrierPos;
    qDebug()<<"rfid"<<rfidNum;

    if (!debugMsg.isEmpty()) debugMsg.clear();

    QString text,str;
    int instruct;
    QString willShowText;

    QList<QString> strlist;
    for(int i =0 ;i < list.count();i++)
    {

        text = list.at(i);          //一条指令

        QStringList  splitText ;
        splitText = text.split("-"); //一条指令列表
        //qDebug()<<splitText<<"指令"<<endl;

        str = splitText.at(0);
        instruct = str.toInt();

        //判断每条命令的种类
        if (IS_BASE_INSTRUCT(instruct))
        {

            splitText.replace(0,QString::number(instruct));

            str = BaseInstructTranslate(splitText,CarrierPos,rfidNum);


            willShowText.append(str+"\r\n");

        }

        if (IS_ADVANCE_INSTRUCT(instruct))
        {

            splitText.replace(0,QString::number(instruct));



            qDebug()<<splitText<<"指令"<<endl;

            strlist = AdvanceInstructTranslate(splitText,CarrierPos,rfidNum);

            str = ListToString(strlist);
            willShowText.append(str);

        }
    }

    qDebug()<<"last"<<willShowText<<endl;

    //输出显示翻译命令时的调试信息
    QString msg =  ListToString(debugMsg);
    MainWindow::GetInstance()->showDebugMsg(msg);

    emit sendData(willShowText);
}
