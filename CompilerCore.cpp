#include "CompilerCommon.h"
#include "CompilerCore.h"
#include <QStringList>
#include <QDebug>

QStringList debugMsg;

#define RFID_BASE_DEV           100

QString STA_Translate()
{
    return "STA";
}

QString END_Translate()
{
    return "END";
}

QString DEY_Translate(int msc)
{
    QString str = "DEY " + QString::number(msc);
    return str;
}

QString MOV_Translate(QList<int>&carrierPos,int leader,int num,int leaderGoal)
{
    QString str = QString("MOV %1 %2 %3").arg(QString::number(leader),QString::number(num),QString::number(leaderGoal));

    //修改carrierPos
    for(int i = 0;i < num;i++)
    {
        carrierPos[(leader - 1) + i] = leaderGoal + i*RFID_BASE_DEV;
    }

    return str;
}

QString CMG_Translate(int num)
{
    QString str = QString("CMG %1").arg(QString::number(num));
    return str;
}


QString BaseInstructTranslate(QStringList src,QList<int> &carrierPos,int rfidNum)
{

    int instPort = (src.at(0)).toInt();
    QString str;
    switch (instPort)
    {
        case INSTRUCT_BASE_STA:
            str = STA_Translate();
            break;
        case INSTRUCT_BASE_END:
            str = END_Translate();
            break;
        case INSTRUCT_BASE_DEY:
            if(src.count() == 2)
            {
                int msc = (src.at(1)).toInt();
                str = DEY_Translate(msc);
            }
            else
                str = "";
            break;
        case INSTRUCT_BASE_MOV:
            if(src.count() == 4)
            {
                int leader = (src.at(1)).toInt();
                int num = (src.at(2)).toInt();
                int leaderGoal = (src.at(3)).toInt();

                if( (leader-1+num) < carrierPos.count())
                {
                    if(!((leaderGoal/RFID_BASE_DEV + num )> rfidNum) && (leaderGoal > 0) && leader <= carrierPos.count())
                    {

                        str = MOV_Translate(carrierPos,leader,num,leaderGoal);
                    }
                    else
                    {
                        debugMsg.append("运动-"+src.at(1)+"-"+src.at(2)+"-"+src.at(3)+" 队头或尾出界");
                        //qDebug()<<"队头或尾出界";

                        //队头或尾出界
                        str = "";
                    }
                 }
                 else
                {
                    debugMsg.append("运动-"+src.at(1)+"-"+src.at(2)+"-"+src.at(3)+" 车辆数过多");
                    //qDebug()<<"车数量过多";

                    //车数量过多
                    str = "";
                }
            }
            else
                str = "";
            break;
        case INSTRUCT_BASE_CMG:
            if(src.count() == 2)
            {
                int num = (src.at(1)).toInt();
                str = CMG_Translate(num);
            }
            else
                str = "";
            break;
        default:
            str = "";
            break;
    }
    return str;
}


/***************************************************************/


QList<QString> SOD_Translate(QList<int> &carrierPos,int rfidNum,int space,int align)
{
    //定义CmdName用于输出调试信息时候引用
    QString cmdName = "间隔分布-"+QString::number(space)+"-"+QString::number(align);

    QList<QString> outList;
    outList.clear();
    if(space < 0)
    {
        debugMsg.append(cmdName+" 间隔分布间距:"+QString::number(space)+"小于0");

        return outList;
    }
    if(!IS_ALIGN_INSTRUCT(align))
    {
        debugMsg.append(cmdName+" 对齐方向:"+QString::number(align)+" 非标");

        return outList;
    }

    int carrierNum = carrierPos.count();

    if((carrierNum + (carrierNum-1)*space) > rfidNum)
    {
        debugMsg.append(cmdName+" 超长");

        qDebug()<<"超长";
        return outList;
    }

    switch (align)
    {
        case INSTRUCT_ALIGN_HEAD:
        {
            if((carrierNum + (carrierNum-1)*space) > (rfidNum - int(carrierPos.at(0)/RFID_BASE_DEV)))
            {

                debugMsg.append(cmdName+" 头对齐间隔分布超长");
                //qDebug()<<"头对齐间隔分布超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierNum));

            //设置头载体车
            QString str = MOV_Translate(carrierPos,1,1,carrierPos.at(0));
            outList.append(str);
            str.clear();

            //推演后续车辆
            for(int index = 1;index < carrierNum;index++)
            {
                //后续车辆位为前车位加间距
                int goal = carrierPos.at(index - 1) + ((space + 1) * RFID_BASE_DEV);
                str = MOV_Translate(carrierPos,index + 1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        case INSTRUCT_ALIGN_MID:
        {
            int middleCarrier = int(double(carrierNum)/2.0 + 0.5);

            //以中间车辆的位置为基础，向前推演出头车目标，再推演后续车辆

            //头车目标 = 中间车辆点位 - 中间车辆左车数 - 中间车辆左总dummy数
            int leaderGoal = int(carrierPos.at(middleCarrier-1)) - ((middleCarrier-1) - (middleCarrier-1)*space)*RFID_BASE_DEV;

            if( leaderGoal <=0 || ((carrierNum + (carrierNum-1)*space) > (rfidNum - leaderGoal / RFID_BASE_DEV)))
            {
                debugMsg.append(cmdName+" 居中对齐间隔分布超长");

                //qDebug()<<"居中对齐间隔分布超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierNum));

            //设置头载体车
            QString str = MOV_Translate(carrierPos,1,1,leaderGoal);
            outList.append(str);
            str.clear();

            //推演后续车辆
            for(int index = 1;index < carrierNum;index++)
            {
                //后续车辆位为前车位加间距
                int goal = carrierPos.at(index - 1) + ((space + 1) * RFID_BASE_DEV);
                str = MOV_Translate(carrierPos,index + 1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        case INSTRUCT_ALIGN_TAIL:
        {
            if((carrierNum + (carrierNum-1)*space) > (int(carrierPos.last()/RFID_BASE_DEV)))
            {
                debugMsg.append(cmdName+" 尾对齐间隔分布超长");

                //qDebug()<<"尾对齐间隔分布超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierNum));

            //设置尾载体车
            QString str = MOV_Translate(carrierPos,carrierNum,1,carrierPos.at(carrierNum-1));
            outList.append(str);
            str.clear();

            //推演前列车辆
            for(int index = (carrierNum - 2);index >= 0;index--)
            {
                //前车位为后车位加间距
                int goal = carrierPos.at(index + 1) - ((space+1) * RFID_BASE_DEV);
                str = MOV_Translate(carrierPos,index + 1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        default:
            break;
    }

    return outList;

}

QList<QString> PUD_Translate(QList<int> &carrierPos,int rfidNum)
{
    //计算最大间隔
    int maxSpace = int((rfidNum - carrierPos.count()-1)/(carrierPos.count() - 1));
    carrierPos[0] = 1 * RFID_BASE_DEV;
    QList<QString> outList = SOD_Translate(carrierPos,rfidNum,maxSpace,INSTRUCT_ALIGN_HEAD);
    return outList;
}

QList<QString> GAT_Translate(QList<int> &carrierPos,int align)
{
    QString cmdName = "集合"+QString::number(align);
    QList<QString> outList;
    outList.clear();
    if(!IS_ALIGN_INSTRUCT(align))
    {
        debugMsg.append(cmdName+" 对齐方向："+align+" 非标");

        //qDebug()<<"align非标";
        return outList;
    }

    switch (align)
    {
        case INSTRUCT_ALIGN_HEAD:
        {
            outList.append(MOV_Translate(carrierPos,1,carrierPos.count(),carrierPos.at(0)));

            break;
        }
        case INSTRUCT_ALIGN_MID:
        {
            int middleCarrier = int(double(carrierPos.count())/2.0 + 0.5);
            int leaderGoal = carrierPos.at(middleCarrier-1) - (middleCarrier - 1)* RFID_BASE_DEV;

            outList.append(MOV_Translate(carrierPos,1,carrierPos.count(),leaderGoal));

            break;
        }
        case INSTRUCT_ALIGN_TAIL:
        {
            int leaderSpace = int(carrierPos.last()/RFID_BASE_DEV) - carrierPos.count();
            outList.append(MOV_Translate(carrierPos,1,carrierPos.count(),(leaderSpace + 1) * RFID_BASE_DEV));
        }
        default:
            break;
    }

    return outList;
}

QList<QString> EXP_Translate(QList<int> &carrierPos,int rfidNum,int rate,int align)
{
    QString cmdName = "膨胀-"+ QString::number(rate) + "-" + QString::number(align);

    QList<QString> outList;
    outList.clear();

    int dummyLen_HeadAndTail = int(((1*rate + ((carrierPos.count()-1)*rate)) * (carrierPos.count()-1))/2);
    int carreirGroupLen_HeadAndTail = dummyLen_HeadAndTail + carrierPos.count();

    if(carreirGroupLen_HeadAndTail > rfidNum && (align==1 || align==3))
    {
        debugMsg.append(cmdName+" 超长");
        //qDebug()<<"超长";
        return outList;
    }

    if(!IS_ALIGN_INSTRUCT(align)) debugMsg.append(cmdName+" 对齐方向非标");

    switch (align)
    {
        case INSTRUCT_ALIGN_HEAD:
        {
            if((carreirGroupLen_HeadAndTail) > (rfidNum - int(carrierPos.at(0)/RFID_BASE_DEV)))
            {
                debugMsg.append(cmdName+" 膨胀头对齐超长");
                //qDebug()<<"膨胀头对齐超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierPos.count()));

            //设置头载体车
            QString str = MOV_Translate(carrierPos,1,1,carrierPos.at(0));
            outList.append(str);
            str.clear();

            //推演后续车辆
            for(int index = 1;index < carrierPos.count();index++)
            {
                //后续车辆等差数量间距
                int goal = carrierPos.at(index-1) + (index * rate + 1) * RFID_BASE_DEV;
                str = MOV_Translate(carrierPos,index+1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        case INSTRUCT_ALIGN_MID:
        {
            //middleCarrier - 自然计数车辆号 ， 在carrierPos这个List中，是以0为开始计数
            int middleCarrier = int(double(carrierPos.count())/2.0 + 0.5);

            int firstGoalCarrierPos = carrierPos.at(middleCarrier-1)-
                    ((rate*(middleCarrier-1)+rate) * (middleCarrier-1)/2 + (middleCarrier-1))*RFID_BASE_DEV;

            int lastGoalCarrierPos = carrierPos.at(middleCarrier-1)+(((carrierPos.count()-
                                   middleCarrier+1)*rate)+(carrierPos.count()-middleCarrier))*RFID_BASE_DEV;

            if((firstGoalCarrierPos<0) || (lastGoalCarrierPos > rfidNum *RFID_BASE_DEV))
            {
                debugMsg.append(cmdName+" 膨胀中对齐超长");
                //qDebug()<<"膨胀中对齐超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierPos.count()));

            //设置中间载体车
            QString str = MOV_Translate(carrierPos,middleCarrier,1,carrierPos.at(middleCarrier-1));
            outList.append(str);
            str.clear();

            //先向左推演
            for(int index = middleCarrier-2;index >=0;index--)
            {
                //前方车辆等差数量间距
                int goal= carrierPos.at(index+1)-((middleCarrier-index-1)*rate+1)*RFID_BASE_DEV;
                str =MOV_Translate(carrierPos,index+1,1,goal);
                outList.append(str);
                str.clear();
            }

            //再向右推演
            for(int index = middleCarrier;index <carrierPos.count();index++)
            {
                //后续车辆等差数量间距
                int goal=carrierPos.at(index-1)+((index-middleCarrier+1)*rate+1)*RFID_BASE_DEV;
                str = MOV_Translate(carrierPos,index+1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        case INSTRUCT_ALIGN_TAIL:
        {
            if((carreirGroupLen_HeadAndTail) > (int(carrierPos.last()/RFID_BASE_DEV)))
            {
                debugMsg.append(cmdName+" 膨胀尾对齐超长");
                //qDebug()<<"膨胀尾对齐超长";
                return outList;
            }

            outList.append(CMG_Translate(carrierPos.count()));

            //设置尾载体车
            QString str = MOV_Translate(carrierPos,carrierPos.count(),1,carrierPos.last());
            outList.append(str);
            str.clear();

            //推演前列车辆
            for(int index = (carrierPos.count() - 2);index >= 0;index--)
            {
                int goal = carrierPos.at(index+1) - ((carrierPos.count() - index - 1) * rate + 1) * RFID_BASE_DEV;
                str = MOV_Translate(carrierPos,index+1,1,goal);
                outList.append(str);
                str.clear();
            }

            break;
        }
        default:
            break;
    }

    return outList;
}

QList<QString> KDM_Translate(QList<int>&carrierPos,int rfidNum,int align )
{
    QList<QString> outList;
    outList.clear();

    int leaderCarPos,goalPos,distance;
    outList.append(CMG_Translate(carrierPos.count()));

    if(IS_ALIGN_INSTRUCT(align))
    {

    switch (align)
    {
        case INSTRUCT_ALIGN_HEAD :
        {
            leaderCarPos=carrierPos.at(0);
            goalPos = 0;
            distance = goalPos -leaderCarPos ;

            for(int index=0;index < carrierPos.count();index++)
            {
                QString str=MOV_Translate(carrierPos,index+1,1,carrierPos.at(index)+distance);
                outList.append(str);
                str.clear();
            }
            break;
        }
        case INSTRUCT_ALIGN_TAIL :
        {
            leaderCarPos=carrierPos.at(carrierPos.count()-1);
            goalPos = rfidNum * RFID_BASE_DEV;
            distance = goalPos -leaderCarPos ;

            for(int index=carrierPos.count()-1;index>=0;index--)
            {
                QString str=MOV_Translate(carrierPos,index+1,1,carrierPos.at(index)+distance);
                outList.append(str);
                str.clear();
            }
            break;
        }
        case INSTRUCT_ALIGN_MID :
        {
            //middleCarrier - 自然计数车辆号 ， 在carrierPos这个List中，是以0为开始计数
            int middleCarrier = int(double(carrierPos.count())/2.0 + 0.5);
            leaderCarPos=carrierPos.at(middleCarrier-1);
            goalPos = int(rfidNum * RFID_BASE_DEV)/2;
            distance = goalPos -leaderCarPos;

            for(int index=0;index<carrierPos.count();index++)
            {
                QString str=MOV_Translate(carrierPos,index+1,1,carrierPos.at(index)+distance);
                outList.append(str);
                str.clear();
            }
            break;
        }
        default:
            break;
    }

    }
    else
    {
        debugMsg.append("保持当前间距移动 对齐方向非标");
    }
    return outList;;
}



QList<QString> AdvanceInstructTranslate(QStringList src,QList<int> &carrierPos,int rfidNum)
{
    int instPort = (src.at(0)).toInt();
    QList<QString> outList;
    outList.clear();

    switch (instPort)
    {
        case INSTRUCT_ADVANCE_SOD:
        {
            if(src.count() == 3)
            {
                int space = (src.at(1)).toInt();
                int align = (src.at(2)).toInt();
                outList.append(SOD_Translate(carrierPos,rfidNum,space,align));
            }

            break;
        }
        case INSTRUCT_ADVANCE_PUD:
        {
            outList.append(PUD_Translate(carrierPos,rfidNum));
            break;
        }
        case INSTRUCT_ADVANCE_GAT:
        {
            if(src.count() == 2)
            {
                int align = (src.at(1)).toInt();
                outList.append(GAT_Translate(carrierPos,align));
            }

            break;
        }
        case INSTRUCT_ADVANCE_EXP:
        {
            if(src.count() == 3)
            {
                int rate = (src.at(1)).toInt();
                int align = (src.at(2)).toInt();
                outList.append(EXP_Translate(carrierPos,rfidNum,rate,align));
            }
            break;
        }
        case INSTRUCT_ADVANCE_KDM:
        {
            if(src.count() == 2)
            {
                int align = (src.at(1)).toInt();
                outList.append(KDM_Translate(carrierPos,rfidNum,align));
            }
            break;
        }
        default:
            break;
    }
    return outList;
}


/***************************************************************/




/*
 *  src--传入指令集
 *  out--转换为casf文件指令集
 */
int InstructTranslate(QList<QString> src, QList<QString> &out, QList<int> &carrierPos, int rfidNum)
{
    out.clear();
    int status = 0;
    int instNum = src.count();

    for(int instIndex = 0;instIndex < instNum;instIndex++)
    {
        QString lineInstruct = src.at(instIndex);
        QStringList lineInstParm =lineInstruct.split(" ");

        //取得命令端口
        int instPort = (lineInstParm.at(0)).toInt();

        //判别命令归属
        if(IS_BASE_INSTRUCT(instPort))
        {
            out.append(BaseInstructTranslate(lineInstParm,carrierPos,rfidNum));
            status++;
        }
        else if(IS_ADVANCE_INSTRUCT(instPort))
        {
            out.append(AdvanceInstructTranslate(lineInstParm,carrierPos,rfidNum));
            status++;
        }

    }
    return status;
}
