#ifndef TABOUTOFDIALOG_H
#define TABOUTOFDIALOG_H

#include <QDialog>

namespace Ui {
class TAboutOfDialog;
}

class TAboutOfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TAboutOfDialog(QWidget *parent = 0);
    ~TAboutOfDialog();

private:
    Ui::TAboutOfDialog *ui;
};

#endif // TABOUTOFDIALOG_H
