#ifndef QTSCANIMAGE_H
#define QTSCANIMAGE_H

#include <QMainWindow>
#include "language_dialog.h"

namespace Ui {
class QtScanImage;
}

class QtScanImage : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtScanImage(QWidget *parent = nullptr);
    ~QtScanImage();

private slots:
    void on_toolButton_getTargetPath_clicked();

    void on_comboBox_size_currentIndexChanged(int index);

    void on_pushButton_scan_clicked();

    void on_pushButton_saveProfile_clicked();

    void on_comboBox_profile_currentTextChanged(const QString &arg1);

    void on_pushButton_convert_clicked();

    void on_pushButton_deleteProfile_clicked();

    void on_lineEdit_saveAsFileName_textChanged(const QString &arg1);

    void on_comboBox_saveAsType_currentTextChanged(const QString &arg1);

    void on_actionLanguage_triggered();
    
    void on_toolButton_getSourceFile_clicked();

private:
    Ui::QtScanImage *ui;
    QString configPath;
    QString langPath;
    void loadProfile();
    void processLine(QString);
    void updateBachCounter();
    void updateProfiles();
    language_dialog lang;
};

#endif // QTSCANIMAGE_H
