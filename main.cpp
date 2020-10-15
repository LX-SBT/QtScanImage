#include "qtscanimage.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QDebug>

QString getLanguage();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString language = getLanguage();
    QTranslator tr;
    if (language != "")
    {
        tr.load(language);
        a.installTranslator(&tr);
    }

    QtScanImage w;
    w.show();

    return a.exec();
}

QString getLanguage()
{
    QString langCfgPath = QDir::homePath() + "/.config/QtScanImage/language.conf";
    QString langPath = QDir::homePath() + "/.config/QtScanImage/languages/";
    QString currentLine = "";
    QFile file(langCfgPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    while (!file.atEnd())
    {
        currentLine = file.readLine();
        currentLine.remove("\n");
        qDebug() << currentLine;
    }
    return langPath + currentLine + ".qm";
}
