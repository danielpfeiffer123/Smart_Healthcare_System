#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_loginBtn_clicked();

    void on_doctorBtn_toggled(bool checked);

    void on_patientBtn_toggled(bool checked);




    void on_registerBtn_clicked();

private:
    Ui::RegisterDialog *ui;
    bool isDoctor;
    bool isPatient;
};

#endif // REGISTERDIALOG_H
