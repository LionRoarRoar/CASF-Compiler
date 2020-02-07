#ifndef INSTURCTDRAWER_H
#define INSTURCTDRAWER_H

#include <QToolBox>
#include <QToolButton>

class InsturctDrawer : public QToolBox
{

    public:
        InsturctDrawer(QWidget *parent=0,Qt::WindowFlags f=0);

    private:
        QToolButton *STA_BUT;
        QToolButton *END_BUT;
        QToolButton *DEY_BUT;
        QToolButton *MOV_BUT;
        QToolButton *CMG_BUT;
        QToolButton *PUD_BUT;
        QToolButton *SOD_BUT;
        QToolButton *GAT_BUT;
        QToolButton *EXP_BUT;
        QToolButton *KDM_BUT;
};

#endif // INSTURCTDRAWER_H
