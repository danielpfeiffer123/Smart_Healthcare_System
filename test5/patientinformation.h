#ifndef PATIENTINFORMATION_H
#define PATIENTINFORMATION_H

#include <QDialog>

namespace Ui {
class PatientInformation;
}

class PatientInformation : public QDialog
{
    Q_OBJECT

public:
    explicit PatientInformation(QWidget *parent = nullptr);
    ~PatientInformation();

private:
    Ui::PatientInformation *ui;
};

#endif // PATIENTINFORMATION_H
