#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include "MultiInputDialog.h"

#define     isEditingFile      1
#define     isOpeningFile      2
#define     isStandby          3

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     static MainWindow* GetInstance();  //获取mainWindow实例
    ~MainWindow();

     QLabel *msgLabel;       //状态栏消息

     void showDebugMsg(QString msg); //输出调试信息

signals:
    void sendListContents(QStringList,QString OriPos,QString Rfid);


public slots:

    void setItem(QStringList textList);

    void newFileCreated(QStringList msgDlgs); //改变标签内容


private slots:
    void receiveText(QString data);  //输入文本

    void on_creatFile_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_buildTextButton_clicked();

    void on_clearTextButton_clicked();

    void exportFile();

    void openFile();

    void changeFileStatus(int fileStatus);

private:

    Ui::MainWindow *ui;

    explicit MainWindow (QWidget *parent = 0); //构造函数 私有化 防止重复实例化

    static MainWindow* m_pMainWindow;       //实例化类指针

    int column_id;

    void Init();        //初始化数据
    void buildConnection(); //建立信号槽链接
    void deleteConnection();
    void clearList();    //清空列表

    QList<QWidget *> buildTab(QString fileName= NULL,QString text = NULL,QStringList msgDlg =QStringList());  //生成新tab页面

    QList<QWidget *> m_TabWidList;

    QString getCurrentTexts();

    MultiInputDialog *m_cmdDialog = NULL;


};

#endif // MAINWINDOW_H
