#include "doctormenu.h"
#include "ui_doctormenu.h"

DoctorMenu::DoctorMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctorMenu)
{
    ui->setupUi(this);
}

DoctorMenu::~DoctorMenu()
{
    delete ui;
}
