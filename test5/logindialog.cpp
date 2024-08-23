#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include "mainwindow.h"
#include "patientmenu.h"
#include "doctormenu.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("im.db");//create database
    if(!db.open()){
        qDebug() << "open DB error";
        return;
    }
    QSqlQuery query;
    query.exec("create table login("
               "id int primary key, "
               "username vchar(32),"
               "password vchar(32))");

    QString qStr = QString("insert into login values(1,'user','123')");
    qDebug() << qStr;
    query.exec(qStr);

    QString username = ui->usrLineEdit->text().trimmed();
    QString password = ui->pwdLineEdit->text();

    query.exec("select username,password from login");
    bool isFound = false;
    while(query.next()){
        if(query.value(0).toString() == username &&
                query.value(1).toString() == password){
            isFound = true;
        }
    }
    db.close();

    //if(ui->usrLineEdit->text().trimmed()=="user" && ui->pwdLineEdit->text()=="123")
    if(isFound && isPatient){
        PatientMenu *w = new PatientMenu;
        w->show();
        this->close();
    }
    else if(isFound && isDoctor){
        DoctorMenu *w = new DoctorMenu;
        w->show();
        this->close();
    }
    else{
        QMessageBox::warning(this,tr("warning"),tr("your username or password is wrong!"),QMessageBox::Yes);
        ui->usrLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->usrLineEdit->setFocus();
    }
}


void LoginDialog::on_registerBtn_clicked()
{
    RegisterDialog *w = new RegisterDialog;
    w->show();
    this->close();
}

void LoginDialog::on_doctorBtn_toggled(bool checked)
{
    isDoctor = checked;
}

void LoginDialog::on_patientBtn_toggled(bool checked)
{
    isPatient = checked;
}
