#include "doctorinformation.h"
#include "ui_doctorinformation.h"

DoctorInformation::DoctorInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoctorInformation)
{
    ui->setupUi(this);
}

DoctorInformation::~DoctorInformation()
{
    delete ui;
}
