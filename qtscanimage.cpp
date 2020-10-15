#include "qtscanimage.h"
#include "ui_qtscanimage.h"
#include <QFile>
#include <QFileDialog>
#include <stdlib.h>
#include <QDebug>

QtScanImage::QtScanImage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtScanImage)
{
    ui->setupUi(this);
    configPath = QDir::homePath() + "/.config/QtScanImage/profiles/";
    ui->label_targetPath->setText(QDir::homePath() + "/");

    // save UI-Default
    on_pushButton_saveProfile_clicked();
    ui->comboBox_profile->clear();

    //QDir configDir(configPath);
    //QStringList configFiles = configDir.entryList(QDir::Files);
    //ui->comboBox_profile->addItems(configFiles);
    //or in one line:
    ui->comboBox_profile->addItems(QDir(configPath).entryList(QDir::Files));
    //qDebug() <<  QDir(configPath).entryList(QDir::Files).count();
    //qDebug() << QDir(configPath);
    updateBachCounter();
    ui->comboBox_profile->setFocus();
}

QtScanImage::~QtScanImage()
{
    delete ui;
}

void QtScanImage::on_toolButton_getTargetPath_clicked()
{
    QString tp = QFileDialog::getExistingDirectory(this,"Target path",QDir::homePath() + "/",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    ui->label_targetPath->setText(tp);
    updateBachCounter();
}

void QtScanImage::on_pushButton_scan_clicked()
{
    //scanimage --resolution 200 --contrast 50 --source ADF --batch=document-p%d.tiff
    QString cmd = "scanimage ";
    cmd += " --mode ";
    switch (ui->comboBox_mode->currentIndex())
    {
        case 0: cmd += "LineArt";break;
        case 1: cmd += "Gray";break;
        case 2: cmd += "Color";break;
    }
    ui->checkBox_source->isChecked() ? cmd += " --source Duplex" : cmd += " --source ADF";
    cmd += " -l " + QString::number(ui->doubleSpinBox_left_offset->value());
    cmd += " -t " + QString::number(ui->doubleSpinBox_top_offset->value());
    cmd += " -x " + QString::number(ui->doubleSpinBox_width->value());
    cmd += " -y " + QString::number(ui->doubleSpinBox_height->value());
    cmd += " --format=" + ui->comboBox_saveAsType->currentText().remove(0,1);
    if (ui->horizontalSlider_resolution->value() != 300) cmd += " --resolution " + QString::number(ui->horizontalSlider_resolution->value());
    if (ui->horizontalSlider_brightness->value() != 0) cmd += " --brightness " + QString::number(ui->horizontalSlider_brightness->value());
    if (ui->horizontalSlider_contrast->value() != 0) cmd += " --contrast " + QString::number(ui->horizontalSlider_contrast->value());
    if (ui->horizontalSlider_red->value() != 0) cmd += " --red-gamma-table " + QString::number(ui->horizontalSlider_red->value());
    if (ui->horizontalSlider_green->value() != 0) cmd += " --green-gamma-table " + QString::number(ui->horizontalSlider_green->value());
    if (ui->horizontalSlider_blue->value() != 0) cmd += " --blue-gamma-table " + QString::number(ui->horizontalSlider_blue->value());
    if (ui->label_targetPath->text() !="")
        cmd += " --batch=" + ui->label_targetPath->text() + "/" + ui->lineEdit_saveAsFileName->text() + ui->comboBox_saveAsType->currentText();
    else
        cmd += " --batch=\"" + ui->lineEdit_saveAsFileName->text() + ui->comboBox_saveAsType->currentText() + "\"";
    cmd += " --batch-start=" + QString::number(ui->spinBox_startCount->value());
    //cmd += " --batch-count=" + QString::number(ui->spinBox_pageCount->value());
    cmd += " --device-name=dsseries:usb:0x04F9:0x60E2";
    qDebug() << cmd;
    system(cmd.toUtf8().data());

    // increment batch-counter by 2 if duplex else by 1
    //ui->spinBox_startCount->setValue(ui->checkBox_source->isChecked() ? ui->spinBox_startCount->value() + 2: ui->spinBox_startCount->value() + 1);
    updateBachCounter();
}

void QtScanImage::on_pushButton_saveProfile_clicked()
{
    if (!QDir(configPath).exists()) QDir().mkdir(configPath);
    QString filePath = configPath + ui->comboBox_profile->currentText();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << "mode=" << ui->comboBox_mode->currentIndex() << "\n";
    out << "source=" << ui->checkBox_source->isChecked() << "\n";
    out << "targetpath=" << ui->label_targetPath->text() << "\n";
    out << "filename=" << ui->lineEdit_saveAsFileName->text() << "\n";
    out << "file_format=" << ui->comboBox_saveAsType->currentText() << "\n";
    out << "paper_size=" << ui->comboBox_size->currentText() << "\n";
    out << "duplex=" << ui->checkBox_source->isChecked() << "\n";
    out << "resolution=" << ui->horizontalSlider_resolution->value() << "\n";
    out << "brightness=" << ui->horizontalSlider_brightness->value() << "\n";
    out << "contrast=" << ui->horizontalSlider_contrast->value() << "\n";
    out << "red=" << ui->horizontalSlider_red->value() << "\n";
    out << "green=" << ui->horizontalSlider_green->value() << "\n";
    out << "blue=" << ui->horizontalSlider_blue->value() << "\n";
    out << "left=" << ui->doubleSpinBox_left_offset->value() << "\n";
    out << "top=" << ui->doubleSpinBox_top_offset->value() << "\n";
    out << "width=" << ui->doubleSpinBox_width->value() << "\n";
    out << "height=" << ui->doubleSpinBox_height->value() << "\n";
    qDebug() << "File saved: " + filePath;
    ui->pushButton_deleteProfile->setEnabled(true);
}

void QtScanImage::on_pushButton_deleteProfile_clicked()
{
    int currentIndex = ui->comboBox_profile->currentIndex();
    QString filePath = configPath + ui->comboBox_profile->currentText();
    QFile file(filePath);
    file.remove();
    ui->comboBox_profile->setCurrentText("Default");
    ui->comboBox_profile->removeItem(currentIndex);
}

void QtScanImage::on_comboBox_profile_currentTextChanged(const QString &arg1)
{
    ui->pushButton_deleteProfile->setDisabled(true);
    ui->pushButton_saveProfile->setDisabled(true);

    if (arg1 != "Default" && ui->comboBox_profile->findText(arg1) > -1)
    {
        ui->pushButton_deleteProfile->setDisabled(false);
    }
    if (arg1 != "Default" && arg1 != "" && !arg1.contains(QRegExp("[<>:;,?\"*|/]")) && !arg1.contains(QRegExp("[\\s]$")))
    {
        ui->pushButton_saveProfile->setDisabled(false);
    }

    // load selected profile
    loadProfile();
}

void QtScanImage::loadProfile()
{
    QString filePath = configPath + ui->comboBox_profile->currentText();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd())
    {
        QString currentLine = file.readLine();
        currentLine.remove("\n");
        //qDebug() << currentLine;
        processLine(currentLine);
    }
    updateBachCounter();
}

void QtScanImage::processLine(QString cl)
{
    // this function is used by the loadProfile()-function
    // to assign the value to the programm at runtime
    if (cl.contains("mode="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->comboBox_mode->setCurrentIndex(cl.toInt());
    }
    if (cl.contains("source="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->checkBox_source->setChecked(cl.toInt());
    }
    if (cl.contains("targetpath="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->label_targetPath->setText(cl);
    }

    if (cl.contains("filename="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->lineEdit_saveAsFileName->setText(cl);
    }
    if (cl.contains("file_format="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->comboBox_saveAsType->setCurrentText(cl);
    }
    if (cl.contains("paper_size="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->comboBox_size->setCurrentText(cl);
    }
    if (cl.contains("duplex="))
    {
        cl.remove(0, cl.indexOf('=')+1);

    }
    if (cl.contains("resolution="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_resolution->setValue(cl.toInt());
    }
    if (cl.contains("brightness="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_brightness->setValue(cl.toInt());
    }
    if (cl.contains("contrast="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_contrast->setValue(cl.toInt());
    }
    if (cl.contains("red="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_red->setValue(cl.toInt());
    }
    if (cl.contains("green="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_green->setValue(cl.toInt());
    }
    if (cl.contains("blue="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->spinBox_blue->setValue(cl.toInt());
    }
    if (cl.contains("left="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->doubleSpinBox_left_offset->setValue(cl.toDouble());
    }
    if (cl.contains("top="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->doubleSpinBox_top_offset->setValue(cl.toDouble());
    }
    if (cl.contains("width="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->doubleSpinBox_width->setValue(cl.toDouble());
    }
    if (cl.contains("height="))
    {
        cl.remove(0, cl.indexOf('=')+1);
        ui->doubleSpinBox_height->setValue(cl.toDouble());
    }
}

void QtScanImage::on_comboBox_size_currentIndexChanged(int index)
{
    ui->doubleSpinBox_width->setEnabled(false);
    ui->doubleSpinBox_height->setEnabled(false);
    ui->doubleSpinBox_top_offset->setEnabled(false);
    ui->doubleSpinBox_left_offset->setEnabled(false);
    ui->doubleSpinBox_top_offset->setValue(0);
    ui->doubleSpinBox_left_offset->setValue(0);

    switch (index)
    {
    case 0: //A4
        ui->doubleSpinBox_width->setValue(210);
        ui->doubleSpinBox_height->setValue(297);
        break;
    case 1: //A5
        ui->doubleSpinBox_width->setValue(148);
        ui->doubleSpinBox_height->setValue(210);
        break;
    case 2: //A6
        ui->doubleSpinBox_width->setValue(105);
        ui->doubleSpinBox_height->setValue(148);
        break;
    case 3: //B5
        ui->doubleSpinBox_width->setValue(176);
        ui->doubleSpinBox_height->setValue(250);
        break;
    case 4: //B6
        ui->doubleSpinBox_width->setValue(125);
        ui->doubleSpinBox_height->setValue(176);
        break;
    case 5: //Letter
        ui->doubleSpinBox_width->setValue(216);
        ui->doubleSpinBox_height->setValue(279);
        break;
    case 6: //Legal
        ui->doubleSpinBox_width->setValue(216);
        ui->doubleSpinBox_height->setValue(356);
        break;
    case 7: //Business card EU
        ui->doubleSpinBox_width->setValue(85);
        ui->doubleSpinBox_height->setValue(55);
        break;
    case 8: //4x6"
        ui->doubleSpinBox_width->setValue(100);
        ui->doubleSpinBox_height->setValue(150);
        break;
    case 9: //5x7"
        ui->doubleSpinBox_width->setValue(130);
        ui->doubleSpinBox_height->setValue(180);
        break;
    case 10: //Custom
        ui->doubleSpinBox_width->setEnabled(true);
        ui->doubleSpinBox_height->setEnabled(true);
        ui->doubleSpinBox_top_offset->setEnabled(true);
        ui->doubleSpinBox_left_offset->setEnabled(true);
        break;
    }
}

void QtScanImage::on_pushButton_convert_clicked()
{
    // convert to pdf & compress
    //convert -quality 60 -compress jpeg out*.png out.pdf
}

void QtScanImage::updateBachCounter()
{
    if(ui->lineEdit_saveAsFileName->text().contains("%d"))
    {
        QStringList matchingBatches = QDir(ui->label_targetPath->text()).entryList(QDir::Files);
        QStringList name_token = (ui->lineEdit_saveAsFileName->text() + ui->comboBox_saveAsType->currentText()).split(QRegExp("%[a-z]"));
        name_token.removeAll("");
        ui->spinBox_startCount->setValue(0);

        for(int i = matchingBatches.count()-1; i >= 0; i--)
        {
            for(int j = 0; j < name_token.count(); j++)
            {
                if(!matchingBatches[i].contains(name_token[j]))
                {
                    matchingBatches.removeAt(i); // if does not contain token, remove item
                    break; // break for-loop at first mismatch. otherwise the search would continue at a deleted index
                }
                else
                    matchingBatches[i].remove(name_token[j]); // else remove token-text from matchingBatches at index i
            }
        }

        for( int i = 0; i < matchingBatches.count(); i++)
        {
            if(ui->spinBox_startCount->value() < matchingBatches[i].toInt())
            {
                ui->spinBox_startCount->setValue(matchingBatches[i].toInt());
            }
        }
        ui->spinBox_startCount->setValue(ui->spinBox_startCount->value()+1);
    }
}

void QtScanImage::on_lineEdit_saveAsFileName_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    updateBachCounter();
}

void QtScanImage::on_comboBox_saveAsType_currentTextChanged(const QString &arg1)
{
    qDebug() << arg1;
    updateBachCounter();
}

void QtScanImage::on_actionLanguage_triggered()
{
    //
}
