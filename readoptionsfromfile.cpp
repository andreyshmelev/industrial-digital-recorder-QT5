#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>
#include <channel1.h>
#include <messages.h>

QString pathtofile = "/usr/";
//QString pathtofile = "C:/Work/";

void Options::readsystemoptionsfromfile()
{
    QFile infile(pathtofile + "systemoptions.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    Options::calibrationprm = json["Calibration"].toString();
    infile.close();
}

void Options::readoptionsfromfile()
{
    QFile infile(pathtofile + "options.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();

    QJsonObject ch1 = array.at(0).toObject();
    options1.SetHigherLimit(ch1.value("HigherLimit").toDouble());
    options1.SetLowerLimit(ch1.value("LowerLimit").toDouble());
    options1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toDouble());
    options1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toDouble());
    options1.SetSignalType(ch1.value("Type").toDouble());
    options1.SetUnitsName(ch1.value("Units").toString());
    options1.SetMeasurePeriod(ch1.value("Period").toDouble());
    options1.SetState1HighMessage(ch1.value("State1HighMessage").toString());
    options1.SetState1LowMessage(ch1.value("State1LowMessage").toString());
    options1.SetState2HighMessage(ch1.value("State2HighMessage").toString());
    options1.SetState2LowMessage(ch1.value("State2LowMessage").toString());
    options1.SetState1Value(ch1.value("State1Value").toDouble());
    options1.SetState2Value(ch1.value("State2Value").toDouble());
    options1.SetChannelName(ch1.value("Name").toString());

    QJsonObject ch2 = array.at(1).toObject();
    options2.SetHigherLimit(ch2.value("HigherLimit").toDouble());
    options2.SetLowerLimit(ch2.value("LowerLimit").toDouble());
    options2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toDouble());
    options2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toDouble());
    options2.SetSignalType(ch2.value("Type").toDouble());
    options2.SetUnitsName(ch2.value("Units").toString());
    options2.SetMeasurePeriod(ch2.value("Period").toDouble());

    QJsonObject ch3 = array.at(2).toObject();
    options3.SetHigherLimit(ch3.value("HigherLimit").toDouble());
    options3.SetLowerLimit(ch3.value("LowerLimit").toDouble());
    options3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toDouble());
    options3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toDouble());
    options3.SetSignalType(ch3.value("Type").toDouble());
    options3.SetUnitsName(ch3.value("Units").toString());
    options3.SetMeasurePeriod(ch3.value("Period").toDouble());

    QJsonObject ch4 = array.at(3).toObject();
    options4.SetHigherLimit(ch4.value("HigherLimit").toDouble());
    options4.SetLowerLimit(ch4.value("LowerLimit").toDouble());
    options4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toDouble());
    options4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toDouble());
    options4.SetSignalType(ch4.value("Type").toDouble());
    options4.SetUnitsName(ch4.value("Units").toString());
    options4.SetMeasurePeriod(ch4.value("Period").toDouble());
    infile.close();
}

QJsonArray MessageWrite::LogMessageRead()
{
    QFile file(pathtofile + "Log.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["messagesqueue"].toArray();
    MessageWrite::messagesqueue = array;
    file.close();
//    qDebug() << MessageWrite::messagesqueue;
    return MessageWrite::messagesqueue;
}

void ChannelOptions::readoptionsfromfile(int channel)
{
    QFile infile(pathtofile + "options.txt");
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    QJsonObject ch = array.at(channel-1).toObject();

    this->SetHigherLimit(ch.value("HigherLimit").toDouble());
    this->SetLowerLimit(ch.value("LowerLimit").toDouble());
    this->SetHigherMeasureLimit(ch.value("HigherMeasLimit").toDouble());
    this->SetLowerMeasureLimit(ch.value("LowerMeasLimit").toDouble());
    this->SetSignalType(ch.value("Type").toDouble());
    this->SetUnitsName(ch.value("Units").toString());
    this->SetMeasurePeriod(ch.value("Period").toDouble());
    this->SetState1HighMessage(ch.value("State1HighMessage").toString());
    this->SetState1LowMessage(ch.value("State1LowMessage").toString());
    this->SetState2HighMessage(ch.value("State2HighMessage").toString());
    this->SetState2LowMessage(ch.value("State2LowMessage").toString());
    this->SetState1Value(ch.value("State1Value").toDouble());
    this->SetState2Value(ch.value("State2Value").toDouble());
    this->SetChannelName(ch.value("Name").toString());

    infile.close();
}
