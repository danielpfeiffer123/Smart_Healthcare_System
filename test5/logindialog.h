#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

    void on_doctorBtn_toggled(bool checked);

    void on_patientBtn_toggled(bool checked);

private:
    Ui::LoginDialog *ui;
    bool isDoctor  = false;
    bool isPatient = false;
};

#endif // LOGINDIALOG_H
