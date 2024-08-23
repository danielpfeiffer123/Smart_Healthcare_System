#include "clientmenubar.h"
#include "ui_clientmenubar.h"

ClientMenubar::ClientMenubar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientMenubar)
{
    ui->setupUi(this);
}

ClientMenubar::~ClientMenubar()
{
    delete ui;
}
