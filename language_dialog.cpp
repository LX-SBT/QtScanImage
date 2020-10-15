#include "language_dialog.h"
#include "ui_language_dialog.h"

language_dialog::language_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::language_dialog)
{
    ui->setupUi(this);
}

language_dialog::~language_dialog()
{
    delete ui;
}
