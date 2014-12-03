#include "taboutofdialog.h"
#include "ui_taboutofdialog.h"

TAboutOfDialog::TAboutOfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAboutOfDialog)
{
    ui->setupUi(this);
}

TAboutOfDialog::~TAboutOfDialog()
{
    delete ui;
}
