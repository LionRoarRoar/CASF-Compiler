#include "insturctdrawer.h"
#include <QGroupBox>
#include <QVBoxLayout>


InsturctDrawer::InsturctDrawer(QWidget *parent,Qt::WindowFlags f)
                :QToolBox(parent,f)
{
    setWindowTitle(tr("基础指令"));
    STA_BUT = new QToolButton;
    STA_BUT->setText(tr("STA"));
//    STA_BUT->setAutoRaise(true);
    END_BUT = new QToolButton;
    END_BUT->setText(tr("END"));
//    END_BUT->setAutoRaise(true);
    QGroupBox *BaseGroup =new QGroupBox;
    QVBoxLayout *layout1 = new QVBoxLayout(BaseGroup);
    layout1->setMargin(10);
    layout1->setAlignment(Qt::AlignHCenter);
    layout1->addWidget(STA_BUT);
    layout1->addWidget(END_BUT);
    layout1->addStretch();

    setWindowTitle(tr("基础指令"));
    PUD_BUT = new QToolButton;
    PUD_BUT->setText(tr("PUD"));
    PUD_BUT->setAutoRaise(true);

    SOD_BUT = new QToolButton;
    SOD_BUT->setText(tr("SOD"));
    SOD_BUT->setAutoRaise(true);

    GAT_BUT = new QToolButton;
    GAT_BUT->setText(tr("GAT"));
    GAT_BUT->setAutoRaise(true);

    EXP_BUT = new QToolButton;
    EXP_BUT->setText(tr("EXP"));
    EXP_BUT->setAutoRaise(true);

    KDM_BUT = new QToolButton;
    KDM_BUT->setText(tr("KDM"));
    KDM_BUT->setAutoRaise(true);

    QGroupBox *AdvanceGroup =new QGroupBox;
    QVBoxLayout *layout2 = new QVBoxLayout(AdvanceGroup);
    layout2->setMargin(10);
    layout2->setAlignment(Qt::AlignHCenter);
    layout2->addWidget(PUD_BUT);
    layout2->addWidget(SOD_BUT);
    layout2->addWidget(GAT_BUT);
    layout2->addWidget(EXP_BUT);
    layout2->addWidget(KDM_BUT);
    layout2->addStretch();


    this->addItem((QWidget*)BaseGroup,tr("基础指令"));
    this->addItem((QWidget*)AdvanceGroup,tr("高级指令"));
}


