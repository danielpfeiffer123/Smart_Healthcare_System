#include "patientinformation.h"
#include "ui_patientinformation.h"

PatientInformation::PatientInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientInformation)
{
    ui->setupUi(this);
}

PatientInformation::~PatientInformation()
{
    delete ui;
}
