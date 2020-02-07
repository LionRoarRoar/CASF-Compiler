#ifndef COMPILERCORE_H
#define COMPILERCORE_H
#include <QList>

extern QStringList debugMsg;  //定义调试信息debugMsg全局变量

int InstructTranslate(QList<QString> src,QList<QString> &out,QList<int> &carrierPos,int rfidNum);

QString STA_Translate();

QString END_Translate();

QString DEY_Translate(int msc);

QString MOV_Translate(QList<int>&carrierPos,int leader,int num,int leaderGoal);

QString CMG_Translate(int num);

QString BaseInstructTranslate(QStringList src,QList<int> &carrierPos,int rfidNum);

QList<QString> SOD_Translate(QList<int> &carrierPos,int rfidNum,int space,int align);

QList<QString> PUD_Translate(QList<int> &carrierPos,int rfidNum);

QList<QString> GAT_Translate(QList<int> &carrierPos,int align);

QList<QString> EXP_Translate(QList<int> &carrierPos,int rfidNum,int rate,int align);

QList<QString> KDM_Translate(QList<int>&carrierPos,int rfidNum,int align );

QList<QString> AdvanceInstructTranslate(QStringList src,QList<int> &carrierPos,int rfidNum);

#endif
