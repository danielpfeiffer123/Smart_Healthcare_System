#ifndef DOCTORINFORMATION_H
#define DOCTORINFORMATION_H

#include <QDialog>

namespace Ui {
class DoctorInformation;
}

class DoctorInformation : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorInformation(QWidget *parent = nullptr);
    ~DoctorInformation();

private:
    Ui::DoctorInformation *ui;
};

#endif // DOCTORINFORMATION_H
