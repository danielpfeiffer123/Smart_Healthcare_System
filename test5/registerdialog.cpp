#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "logindialog.h"
#include "patientinformation.h"
#include "doctorinformation.h"
#include "mainwindow.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}


void RegisterDialog::on_loginBtn_clicked()
{
    LoginDialog *w = new LoginDialog;
    w->show();
    this->close();
}



void RegisterDialog::on_doctorBtn_toggled(bool checked)
{
    isDoctor = checked;
}

void RegisterDialog::on_patientBtn_toggled(bool checked)
{
    isPatient = checked;
}
