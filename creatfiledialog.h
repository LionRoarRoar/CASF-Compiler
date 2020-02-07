#ifndef CREATFILEDIALOG_H
#define CREATFILEDIALOG_H

#include <QDialog>

namespace Ui {
class creatFileDialog;
}

class creatFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit creatFileDialog(QWidget *parent = 0);
    ~creatFileDialog();

private slots:

    void on_buttonBox_accepted();

private:
    Ui::creatFileDialog *ui;

signals:
    void sendText(QString);
    void requestClearText();
    void requestSaveData(QString,QString);
};

#endif // CREATFILEDIALOG_H
