#ifndef LANGUAGE_DIALOG_H
#define LANGUAGE_DIALOG_H

#include <QDialog>

namespace Ui {
class language_dialog;
}

class language_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit language_dialog(QWidget *parent = nullptr);
    ~language_dialog();

private:
    Ui::language_dialog *ui;
};

#endif // LANGUAGE_DIALOG_H
