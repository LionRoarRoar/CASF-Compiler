#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "ShowText.h"
#include <synchapi.h>
#include <QTableWidgetItem>
#include <QLabel>
#include "CompilerCommon.h"
#include<QUrl>
#include<QFile>
#include<QFileDialog>
#include <QMessageBox>


MainWindow* MainWindow::m_pMainWindow = NULL;

MainWindow* MainWindow::GetInstance()
{
    if(m_pMainWindow == NULL)
        m_pMainWindow = new MainWindow() ;

    return m_pMainWindow;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();

    buildConnection();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pMainWindow;
    deleteConnection();
}

void MainWindow::Init()
{
    column_id=0;

    msgLabel = new QLabel;

    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignLeft);

    statusBar()->addWidget(msgLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    ui->debugMsg->resize(ui->tab->width(),ui->tab->height());

    changeFileStatus(isStandby);

    //设置初始List和filetabwid界面比例
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 5);
}

void MainWindow::buildConnection()
{

    ShowText* showlistContents = new ShowText ;
    //翻译item列表为文本
    connect(this,MainWindow::sendListContents,showlistContents,ShowText::transListContents);

    //显示文本数据
    connect(showlistContents,ShowText::sendData,this,MainWindow::receiveText);


    connect(ui->exportFile,&QAction::triggered,this,&MainWindow::exportFile);

    connect(ui->openFile,&QAction::triggered,this,&MainWindow::openFile);

}

void MainWindow::deleteConnection()
{
    disconnect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

    delete m_cmdDialog ;

    m_cmdDialog = NULL;
}


void MainWindow::showDebugMsg(QString msg)
{
    ui->debugMsg->clear();
    ui->debugMsg->append(msg);
}


void MainWindow::on_creatFile_triggered()
{
    MultiInputDialog* creatfileDialog = new MultiInputDialog(3);



    QStringList list;

    list << "车辆当前位置：" << "RFID点位数: "<<"文件名：" ;

    creatfileDialog->SetLabelTexts(list);

    creatfileDialog->SetLabelsWidth(100);

    //当对话框Ok按下时候保存用户输入数据
    // connect(creatfileDialog,MultiInputDialog::sendData,showlistContents,ShowText::saveData);


    //修改标签文本显示
    connect(creatfileDialog,MultiInputDialog::sendData,this,MainWindow::newFileCreated);

    creatfileDialog->show();


}



void  MainWindow::receiveText(QString data)
{
    int index = ui->fileTabWid->currentIndex();
    QTextEdit *textEdit = (QTextEdit *)m_TabWidList.at((index+1)*3-1);
    textEdit->clear();
    textEdit->append(data);

}


QString MainWindow::getCurrentTexts()
{
    int index = ui->fileTabWid->currentIndex();
    QTextEdit *textEdit = (QTextEdit *)m_TabWidList.at((index+1)*3-1);
    return textEdit->toPlainText();
}

void MainWindow::clearList()
{
    column_id = 0 ;

    ui->listWidget->clear();
}

//基础指令双击响应
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //判断双击项目名称
    if(item->text(column)=="运动")
    {

        m_cmdDialog = new MultiInputDialog(3);
        QStringList list;

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        list << "车头编号:" << "车队数量:" << "头车目标位置:" ;
        m_cmdDialog->SetLabelTexts(list);
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("运动");

        m_cmdDialog->show();

    }

    if(item->text(column)=="开始")
    {

        QStringList list;

        list.append("开始");
        setItem(list);
    }

    if(item->text(column)=="结束")
    {

        QStringList list;

        list.append("结束");
        setItem(list);
    }

    if(item->text(column)=="延时")
    {

        m_cmdDialog = new MultiInputDialog(1);

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"延时时间（秒）：");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("延时");

        m_cmdDialog->show();
    }

    if(item->text(column)=="打包")
    {
        m_cmdDialog = new MultiInputDialog(1);
        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"并发命令条数：");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("打包");

        m_cmdDialog->show();
    }

    if(item->text(column)=="轨道均布")
    {

        QStringList list;

        list.append("轨道均布");
        setItem(list);
    }

    if(item->text(column)=="间隔分布")
    {

        m_cmdDialog = new MultiInputDialog(2);

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"间隔距离");
        m_cmdDialog->SetOneLabelText(1,"对齐方向");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("间隔分布");

        m_cmdDialog->show();
    }

    if(item->text(column)=="集合")
    {

        m_cmdDialog = new MultiInputDialog(1);

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"对齐方向：");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("集合");

        m_cmdDialog->show();
    }

    if(item->text(column)=="膨胀")
    {

        m_cmdDialog = new MultiInputDialog(2);

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"膨胀比例");
        m_cmdDialog->SetOneLabelText(1,"对齐方向");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("膨胀");

        m_cmdDialog->show();
    }

    if(item->text(column)=="保持当前间距移动")
    {

        m_cmdDialog = new MultiInputDialog(1);

        connect(m_cmdDialog,MultiInputDialog::sendData,this,MainWindow::setItem);

        m_cmdDialog->SetOneLabelText(0,"移动方向：");
        m_cmdDialog->SetLabelsWidth(100);
        m_cmdDialog->setWindowTitle("保持当前间距移动");

        m_cmdDialog->show();
    }

}



QMap<QString,int> map_StrlistToCode =
{
    {"开始",INSTRUCT_BASE_STA},
    {"结束",INSTRUCT_BASE_END},
    {"运动",INSTRUCT_BASE_MOV},
    {"延时",INSTRUCT_BASE_DEY},
    {"打包",INSTRUCT_BASE_CMG},
    {"轨道均布",INSTRUCT_ADVANCE_PUD},
    {"间隔分布",INSTRUCT_ADVANCE_SOD},
    {"集合",INSTRUCT_ADVANCE_GAT},
    {"膨胀",INSTRUCT_ADVANCE_EXP},
    {"保持当前间距移动",INSTRUCT_ADVANCE_KDM}
};


QList<QWidget *> MainWindow::buildTab(QString fileName,QString text,QStringList msgDlg)
{
    QWidget *tab = new QWidget();

    QVBoxLayout *VBLayoutTab = new QVBoxLayout();

    QTextEdit *textEdit = new QTextEdit(tab);
    textEdit->setFontPointSize(12);

    QList<QWidget *> widgetList;
    if (fileName!=NULL) //fileName不为空 则为打开文件
    {
        int index =ui->fileTabWid->addTab(tab,fileName);
        ui->fileTabWid->setCurrentIndex(index);
        if(text != NULL)
        {
            textEdit->setText(text);
        }

        widgetList<<nullptr<<nullptr<<textEdit;

    }

    else if(msgDlg.size()!=0 )   //msgDlg不为空 则为新建文件
    {
        QLabel *orignalPosLabel_1 = new QLabel(tab);
        orignalPosLabel_1->setText(tr("初始位置："));
        QLabel *orignalPosLabel_2 = new QLabel(tab);
        orignalPosLabel_2->setText(msgDlg.at(1));

        QLabel *endPosLabel_1 = new QLabel(tab);
        endPosLabel_1->setText(tr("初始RFID数目："));
        QLabel *endPosLabel_2 = new QLabel(tab);
        endPosLabel_2->setText(msgDlg.at(2));

        fileName =msgDlg.at(3);

        int index =ui->fileTabWid->addTab(tab,fileName);
        ui->fileTabWid->setCurrentIndex(index);

        QHBoxLayout *HBLayout_1 = new QHBoxLayout();
        QHBoxLayout *HBLayout_2 = new QHBoxLayout();

        HBLayout_1->addWidget(orignalPosLabel_1);
        HBLayout_1->addWidget(orignalPosLabel_2);
        HBLayout_1->addStretch();

        HBLayout_2->addWidget(endPosLabel_1);
        HBLayout_2->addWidget(endPosLabel_2);
        HBLayout_2->addStretch();

        VBLayoutTab->addLayout(HBLayout_1);
        VBLayoutTab->addLayout(HBLayout_2);

        widgetList<<orignalPosLabel_2<<endPosLabel_2<<textEdit;
    }

    VBLayoutTab->addWidget(textEdit);
    VBLayoutTab->addStretch();

    tab->setLayout(VBLayoutTab);

    //返回一个长度为3的控件指针列表
    return widgetList;
}
void MainWindow::changeFileStatus(int fileStatus)
{
    switch (fileStatus)
    {
    case isEditingFile:
    {
        ui->treeWidget->setEnabled(1);

        ui->buildTextButton->setEnabled(1);

        ui->clearTextButton->setEnabled(1);
        break;
    }
    case isOpeningFile:
    {
        ui->treeWidget->setEnabled(1);
\
        ui->buildTextButton->setEnabled(1);

        ui->clearTextButton->setEnabled(1);

        break;
    }
    case isStandby:
    {
        ui->treeWidget->setEnabled(0);

        ui->buildTextButton->setEnabled(0);

        ui->clearTextButton->setEnabled(0);
        break;
    }
    default:
        break;
    }

}

void MainWindow::newFileCreated(QStringList msgDlgs)
{
    changeFileStatus(isEditingFile);

    m_TabWidList.append( buildTab(NULL,NULL,msgDlgs) );

    msgLabel->setText(tr("新建文件成功"));
}

void MainWindow::setItem(QStringList textList)
{
    QString  text;
    QStringList  contents; //把中文命令转为实际指令
    QListWidgetItem* tempItem;
    QVariant data;
    for(int i=0;i<textList.count();i++)
    {
        text.append(textList.at(i));
        text.append("-");
    }
    text.chop(1);

    contents = textList;
    contents.prepend(QString::number(map_StrlistToCode[contents.at(0)]));
    contents.removeAt(1);

    ui->listWidget->addItem(text);
    ui->listWidget->setIconSize(QSize(50,50));
    tempItem = ui->listWidget->item(column_id++);
    tempItem->setData(Qt::UserRole,column_id);
    tempItem->setData(Qt::UserRole+1,contents);

    data = tempItem->data(Qt::UserRole+1);
    qDebug()<<text<<endl;
    qDebug()<<data<<endl;

}

void MainWindow::on_buildTextButton_clicked()
{
    QStringList itemTexts ;
    for(int i=0;i<ui->listWidget->count();i++)
    {
        QStringList data = (ui->listWidget->item(i)->data(Qt::UserRole+1)).toStringList();
        QString itemText;
        for(int index=0 ;index<data.count(); index++)
        {
            itemText.append(data.at(index)+"-");
        }
        itemText.chop(1);

        itemTexts.append(itemText);
    }

    qDebug()<<itemTexts<<endl;

    int index = ui->fileTabWid->currentIndex();
    QLabel *OriPosLab = (QLabel *)m_TabWidList.at(index*3);
    QLabel *RfidLab = (QLabel *)m_TabWidList.at((index+1)*3-2);
    QString OriPos = OriPosLab->text();
    QString Rfid = RfidLab->text();

    emit sendListContents(itemTexts,OriPos,Rfid); //发送itemlist中所有Item


}

void MainWindow::on_clearTextButton_clicked()
{
    //清除listwidget所有Item前要先解除所有信号连接 并将item的id归零
    //不然清除后再创建item项目会崩溃
    if(m_cmdDialog != NULL) deleteConnection();

    if(column_id!=0)  clearList();
}

void MainWindow::exportFile()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("导出文本"),"",tr("脚本格式 (*.casf)"));


    if(!filename.isNull())
    {
        qDebug()<<"filename:"<<filename;

        QString text = getCurrentTexts();

        QFile file(filename);            //新建file对象

        if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            QMessageBox::warning(this,"file write","can't open",QMessageBox::Yes);
        }
        else
        {
            QTextStream in(&file);           //写入文本

            in<<text;

            msgLabel->setText(tr("导出文件成功"));
        }

    }
    else return;


}


void MainWindow::openFile()
{

    QString File = QFileDialog::getOpenFileName(this,tr("打开文件"),"",tr("脚本格式 (*.casf)"));

    QFileInfo fi = QFileInfo(File);   //文件信息

    QString fileName =fi.fileName();

    if(!File.isNull())
    {
        qDebug()<<"filename:"<<File;

        if(!QFile::exists(File))
        {
            QMessageBox::warning(this,"file load","file not exist",QMessageBox::Yes);
        }

        QFile file(File);            //新建file对象

        if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            QMessageBox::warning(this,"file write","can't open",QMessageBox::Yes);
        }

        else
        {
            QString text = file.readAll();

            buildTab(fileName,text);

            msgLabel->setText(tr("打开文件成功"));

            changeFileStatus(isOpeningFile);  //使能textEdit和treeWidget
        }
    }
    else return;


}
