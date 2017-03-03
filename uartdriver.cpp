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
#include <uartdriver.h>

extern QString inputstr ;

double UartDriver::channelinputbuffer[] = {27.22,33.87,57.89,81.11};

void UartDriver::readuart()
{
    char arr[9] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD, '\n'};
    QString ba2 = "01030000000AC5CD";

    QByteArray requestData;
    // 01 03 00 00 00 0A C5 CD
    QByteArray ba(arr, 8);

    while (1)
    {
        QSerialPort serial;
        serial.setPortName("COM3"); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            qDebug() << serial.portName() + " Opened";

            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);


            //qDebug() << "serial.bytesAvailable: " ;
            //qDebug() << serial.bytesAvailable();

            while (1)
            {
                serial.write(ba);
                while (serial.waitForBytesWritten(10))
                    ;
//                serial.write(QByteArray::fromHex("01030000000AC5CD"));
//                while (serial.waitForBytesWritten(20))
//                    ;

                Sleep(100);
                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();

                qDebug() << "recieve: " + requestData;
            }

        }
    }
}

void UartDriver::writechannelvalue(int channel, double value)
{
    this->channelinputbuffer[channel-1] = value;
    //    qDebug() << "writechannelvalue";
}

void UartDriver::just()
{
    //    qDebug() << "just";
}

double UartDriver::readchannelvalue(int channelnumber)
{
    return channelinputbuffer[channelnumber-1];
}

QByteArray UartDriver::ReadAllUartByteData()
{
    QSerialPort serial;
    QByteArray bytedata;
    //    serial.setPortName("/dev/ttyS1"); //usart1
    serial.setPortName("COM3"); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {

        //        serial.waitForReadyRead(10);

        qDebug() << serial.bytesAvailable();

        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        while (serial.waitForReadyRead(10))
            bytedata.append( serial.readAll() );
    }

    return bytedata;
}

QString UartDriver::ReadAllUartStringData()
{
    QSerialPort serial;
    QByteArray bytedata;
    //    serial.setPortName("/dev/ttyS1"); //usart1
    serial.setPortName("COM1"); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        while (serial.waitForReadyRead(10))
            bytedata.append( serial.readAll() );
    }

    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);

    return DataAsString;
}

QString UartDriver::readalluartports()
{
    QString a;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

        {    // Example use QSerialPort
            QSerialPort serial;
            serial.setPort(info);
            a.append(info.portName());
        }
    }

    return a;
}

void UartDriver::writedata()
{
    //01 03 00 00 00 0A C5 CD

    char arr[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};

    // 01 03 00 00 00 0A C5 CD

    QByteArray ba(arr, 8);

    QByteArray bytedata;

    QSerialPort serial;

    //    serial.setPortName("/dev/ttyS1"); //usart1
    serial.setPortName("COM3"); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::SoftwareControl);

        //        serial.write("1");
        serial.write(ba);

        while (serial.waitForBytesWritten(100))
            ;
        Sleep(100);

        qDebug() << "bytesAvailable" + serial.bytesAvailable();

        while (serial.waitForReadyRead(100))
            bytedata.append( serial.readAll() );

        qDebug() << "bytesAvailable" + serial.bytesAvailable();
    }
    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);
    serial.close();
}