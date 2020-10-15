#ifndef LANGUAGE_DIALOG_H
#define LANGUAGE_DIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class language_dialog;
}

class language_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit language_dialog(QWidget *parent = nullptr);
    ~language_dialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::language_dialog *ui;
    void update_lang();
};

#endif // LANGUAGE_DIALOG_H
