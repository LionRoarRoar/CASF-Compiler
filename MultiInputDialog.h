#ifndef MULTIINPUTDIALOG_H
#define MULTIINPUTDIALOG_H

#include<QWidget>
#include<QDialog>
#include<QLabel>
#include <QLineEdit>

class MultiInputDialog : public QDialog
{
    Q_OBJECT
private:
    const int m_GroupCount;

    QLabel **m_Labels;
    QLineEdit **m_LineEdits;

    QPushButton *m_CancelButton;

signals:
    void sendData(QStringList);

public:
    QPushButton *m_OKButton;

    bool Ok_Button_Down;  //判断对话框是否Ok键按下

    MultiInputDialog(int count, QWidget *parent = 0);
    virtual ~MultiInputDialog();
    void SetLabelTexts(const QStringList &listText);
    void SetOneLabelText(int index, const QString &text);
    QString GetOneText(int index);
    QStringList GetAllTexts();
    //为了让多组输入框看起来整齐些，所以我固定了QLabel的宽度
    void SetLabelsWidth(int width);
    //使用正则表达式来限制输入的字符
    void SetLineEditRegExp(int index, QRegExp regExp);

public slots:
    //请根据需要重写这两个函数
    void m_acccept();
    QStringList receiveData(QStringList list);

    //void reject() { QDialog::reject(); }
};


#endif // MULTIINPUTDIALOG_H
