#include "language_dialog.h"
#include "ui_language_dialog.h"

#include <QDir>

language_dialog::language_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::language_dialog)
{
    ui->setupUi(this);
    update_lang();
}

language_dialog::~language_dialog()
{
    delete ui;
}

void language_dialog::update_lang()
{
    QString langPath = QDir::homePath() + "/.config/QtScanImage/languages/";

    ui->comboBox_language->addItem("English");
    ui->comboBox_language->addItems(QDir(langPath).entryList(QDir::Files));

    for (int i = 0; i < ui->comboBox_language->count() ; i++)
    {
        ui->comboBox_language->setItemText(i, ui->comboBox_language->itemText(i).remove(".qm"));
    }
}

void language_dialog::on_buttonBox_accepted()
{
    QString langCfgPath = QDir::homePath() + "/.config/QtScanImage/language.conf";

    //qDebug() << ui->comboBox_language->currentText();
    if(ui->comboBox_language->currentText() == "English")
    {
        //delete "language"-file
        QFile::remove(langCfgPath);
    }
    else
    {
        QFile langCfg(langCfgPath);
        if (!langCfg.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&langCfg);
        out << ui->comboBox_language->currentText();
    }
}
