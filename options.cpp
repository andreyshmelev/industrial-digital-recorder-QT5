#include "options.h"
#include "ui_options.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

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
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QtTest/QTestEvent>
#include <QtTest/QTestEventList>
#include <QtTest/QTest>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>

#include <QFocusEvent>

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));
    connect(ui->radioButton, SIGNAL(pressed()), this, SLOT(checkboxchange()) );
}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    WriteOptionsToFile();
//    savesettings();
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}

void Options::checkboxchange()
{
    ui->lineEdit_5->setText("newstring");
}

void Options::on_radioButton_2_clicked()
{
}

void Options::on_radioButton_3_clicked()
{
}
int Options::GetSignalType()
{
    if ( ui->radioButton->isChecked() )
            return 1;

    if ( ui->radioButton_2->isChecked() )
            return 2;
    if ( ui->radioButton_3->isChecked() )
            return 3;
    if ( ui->radioButton_4->isChecked() )
            return 4;
    if ( ui->radioButton_5->isChecked() )
            return 5;
}

QString Options::GetSignalUnits()
{
            return "Signal units";
}