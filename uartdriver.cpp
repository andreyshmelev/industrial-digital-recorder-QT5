#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "channel1.h"
#include "uartdriver.h"
#include <QFile>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>

#define BeagleBone

#ifdef BeagleBone
#define comportname "/dev/ttyS1"
QString pathtofile = "/usr/test/";
#define uartsleep DelayMsec(50);
#define longsleep delay(1000);
#endif

#ifndef BeagleBone
#define comportname "COM3"
#define uartsleep Sleep(50);
#define longsleep Sleep(1000);
QString pathtofile = "C:/Work/";
#endif

extern QString inputstr;

double UartDriver::channelinputbuffer[4];
double UartDriver::channeltempbuffer[4];
bool UartDriver::needtoupdatechannel[4] = {0,0,0,0};

uint ModBus::ConnectFailure =false;

quint16 ModBus::Calculate_crc16_modbus(const QByteArray &array)
{
    static const quint16 wCRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040};

    quint8 nTemp;
    quint16 wCRCWord = 0xFFFF;

    for (int i=0; i < array.length();++i)
    {
        nTemp = (quint8)array.at(i) ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord ^= wCRCTable[nTemp];
    }
    //    qDebug() << wCRCWord;
    return wCRCWord;
}

void UartDriver::writechannelvalue(int channel, double value)
{
    this->channelinputbuffer[channel-1] = value;
}

QByteArray UartDriver::ReadAllUartDataByteFormat()
{
    QSerialPort serial;
    QByteArray bytedata;
    serial.setPortName(comportname); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        while (serial.waitForReadyRead(10))
        {
            bytedata.append( serial.readAll() );

        }
    }
    return bytedata;
}

QString UartDriver::ReadAllUartDataStringFormat()
{
    QSerialPort serial;
    QByteArray bytedata;

    serial.setPortName(comportname); //usart1
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

QString UartDriver::ReadAllAvailableCOMPorts()
{
    QString a;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        {
            QSerialPort serial;
            serial.setPort(info);
            a.append(info.portName());
        }
    }
    return a;
}

void UartDriver::DelayMsec(int n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        ;
}

void UartDriver::SetRTS(bool newstate)
{
#ifdef BeagleBone
    QFile file(GetPathToRTSPinValue());
    QTextStream out(&file);
    file.open(QIODevice::WriteOnly);
    out << newstate;
    file.close();
#endif
}

void  UartDriver::SetRTSPinDirection()
{
#ifdef BeagleBone
    QFile filedir(GetPathToRTSPinDirection());
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
#endif
}

double ModBus::ReadTemperature(char channel)
{
    QByteArray arr;
    QByteArray RequestRespose;
    float val;
    RequestRespose = ModBusMakeRequest(channel,ModBus::ReadInputRegisters,ModBus::TemperetureAdress,ModBus::TemperetureRegLenght);
    arr.resize(4);
    arr[0] = RequestRespose.at(5);
    arr[1] = RequestRespose.at(6);
    arr[2] = RequestRespose.at(3);
    arr[3] = RequestRespose.at(4);
    //convert hex to double
    QDataStream stream(arr);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream >> val;
    return val;
}

double ModBus::ReadVoltage(char channel)
{
    return DataChannelRead(ModBus::UniversalChannel1);
}

float ModBus::ModBusGetValue(char DeviceAdress,char Function,uint16_t Address,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,LenghtHi,LenghtLo,CRC16Hi,CRC16Lo;
    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    LenghtHi = (int) ((Lenght & 0xFF00)>>8);
    LenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(LenghtHi);
    requestdata.append(LenghtLo);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo);
    requestdata.append(CRC16Hi);
    InputDataByteArray  = UartWriteData(requestdata); // make request and recieve response

    QByteArray InputDataByteArrayNoCRCnew = InputDataByteArray;
    InputDataByteArrayNoCRCnew.remove(InputDataByteArray.length()-2,2);
    quint8 inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
    quint8 inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
    quint16 inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo );

    ModBus modb;
    quint16 crc = modb.Calculate_crc16_modbus(InputDataByteArrayNoCRCnew);

    if (InputDataByteArray.length()!=0)
        SetConnectFailure(0);
    else
    {
        SetConnectFailure(1);
        return 0;
    }

    if (inpcrc == crc)
    {
        SetConnectFailure(0);
        QByteArray arr;
        float val;

        arr.resize(4);
        arr[0] = InputDataByteArray.at(5);
        arr[1] = InputDataByteArray.at(6);
        arr[2] = InputDataByteArray.at(3);
        arr[3] = InputDataByteArray.at(4);
        //convert hex to double
        QDataStream stream(arr);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
        stream >> val;
        return val;
    }
    else
    {
        SetConnectFailure(2);
        return 0;
    }
    return 0;
}

QByteArray ModBus::ModBusMakeRequest(char DeviceAdress,char Function,uint16_t Address,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,LenghtHi,LenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    LenghtHi = (int) ((Lenght & 0xFF00)>>8);
    LenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(LenghtHi);
    requestdata.append(LenghtLo);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);
    requestdata.append(CRC16Hi);
    requestdata.append(CRC16Lo);

    InputDataByteArray  = UartWriteData(requestdata);

    QByteArray InputDataByteArrayNoCRCnew = InputDataByteArray;
    InputDataByteArrayNoCRCnew.remove(InputDataByteArray.length()-2,2);
    quint8 inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
    quint8 inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
    quint16 inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo );

    quint16 crc = Calculate_crc16_modbus(InputDataByteArrayNoCRCnew);

    if (inpcrc == crc)
    {
        return InputDataByteArray;// qDebug() << "CRC GOOD";
    }
    else
    {
        return 0;// qDebug() << "CRC BAD";
    }
    return 0;
}

QByteArray ModBus::ModBusMakeRequest(char DeviceAdress,char Function,uint16_t Address,uint16_t AddressBias,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,LenghtHi,LenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) (((Address + AddressBias) & 0xFF00)>>8);
    AddressLo = (int) ((Address + AddressBias) & 0x00FF);
    LenghtHi = (int) ((Lenght & 0xFF00)>>8);
    LenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(LenghtHi);
    requestdata.append(LenghtLo);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);
    requestdata.append(CRC16Hi);
    requestdata.append(CRC16Lo);
    InputDataByteArray  = UartWriteData(requestdata);

    QByteArray InputDataByteArrayNoCRCnew = InputDataByteArray;
    InputDataByteArrayNoCRCnew.remove(InputDataByteArray.length()-2,2);
    quint8 inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
    quint8 inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
    quint16  inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo );
    quint16  crc = Calculate_crc16_modbus(InputDataByteArrayNoCRCnew);

    if (inpcrc == crc)
    {
        // qDebug() << "CRC GOOD";
        return InputDataByteArray;
    }
    else
    {
        // qDebug() << "CRC BAD";
        return 0;
    }
    return 0;
}

QByteArray UartDriver::UartWriteData(QByteArray data)
{
    QByteArray InputDataByteArray;
    QSerialPort serial;
    serial.setPortName(comportname); //usart1

    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        SetRTS(1);
        serial.write(data);
        while (serial.waitForBytesWritten(10))
            ;
        SetRTS(0);
        uartsleep;

        while (serial.waitForReadyRead(10))
            InputDataByteArray = serial.readAll();

        QByteArray InputDataByteArrayNoCRC = InputDataByteArray;
        InputDataByteArrayNoCRC.remove(InputDataByteArray.length()-2,2);
        uint8_t inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
        uint8_t inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
        uint16_t inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo);

        ModBus modb;
        uint16_t crc = modb.Calculate_crc16_modbus(InputDataByteArrayNoCRC);

        if (inpcrc == crc)
        {
            return InputDataByteArray;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

double ModBus::DataChannel1Read()
{
    return DataChannelRead(ModBus::UniversalChannel1);
}

double ModBus::DataChannelRead (char channel)
{
    return ModBusGetValue(channel,ModBus::ReadInputRegisters,ModBus::DataChannel1,ModBus::DataChannelLenght);
}

void ModBus::ReadAllChannelsThread ()
{
    UartDriver UD;
    double currentdata;
    while (1)
    {
        //        uartsleep;
        if (UartDriver::needtoupdatechannel[0] == 1)
        {
            UartDriver::needtoupdatechannel[0] = 0;
            while (currentdata==0)
                currentdata = DataChannelRead(ModBus::UniversalChannel1);
            //        if (ch1.IsMathematical())
            //    {
            //        currentdata = mathres.Solve(ch1.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
            //    }

            if (currentdata!=0)
                UD.writechannelvalue(1,currentdata);
            //            Sleep(100);
        }

        currentdata=0;

        if (UartDriver::needtoupdatechannel[1] == 1)
        {
            UartDriver::needtoupdatechannel[1] = 0;
            while (currentdata==0)
                currentdata = DataChannelRead(ModBus::UniversalChannel1);
            //        if (ch1.IsMathematical())
            //    {
            //        currentdata = mathres.Solve(ch1.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
            //    }
            if (currentdata!=0)
                UD.writechannelvalue(2,currentdata);
            //            Sleep(100);
        }

        currentdata=0;

        if (UartDriver::needtoupdatechannel[2] == 1)
        {
            UartDriver::needtoupdatechannel[2] = 0;
            while (currentdata==0)
                currentdata = DataChannelRead(ModBus::UniversalChannel1);
            //        if (ch1.IsMathematical())
            //    {
            //        currentdata = mathres.Solve(ch1.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
            //    }
            if (currentdata!=0)
                UD.writechannelvalue(3,currentdata);
            //            Sleep(100);
        }

        currentdata=0;

        if (UartDriver::needtoupdatechannel[3] == 1)
        {
            UartDriver::needtoupdatechannel[3] = 0;

            while (currentdata==0)
                currentdata = DataChannelRead(ModBus::UniversalChannel1);
            if (currentdata!=0)
                UD.writechannelvalue(4,currentdata);
        }

        currentdata=0;
    }
    thread()->deleteLater();
}
