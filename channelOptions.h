#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>
#include <QColor>
#include <QMutex>
#include <QVector>
#include "mathresolver.h"

class ChannelOptions: public QObject
{
    Q_OBJECT
public:

    // constructor
    explicit ChannelOptions();
public:

    uint16_t GetSignalType();
    int GetDiapason();
    int GetRegistrationType();
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();
    double GetMeasurePeriod();
    double GetState1Value();
    double GetState2Value();
    double GetDempherValue();

    double GetCurrentChannelValue();
    double GetMaximumChannelValue();
    double GetMinimumChannelValue();
    double GetAverageChannelValue();
    double GetMaxplusMinChannelValue();
    double GetDempheredChannelValue();
    double GetValuePercent();
    double ConvertSignalToValue(double signal);

    QString GetUnitsName();
    QString GetState1HighMessage();
    QString GetState1LowMessage();
    QString GetState2HighMessage();
    QString GetState2LowMessage();
    QString GetChannelName();
    QString GetMathString();
    QColor GetStateDependentColor();
    QColor GetNormalColor();
    QColor GetMaximumColor();
    QColor GetMinimumColor();
    bool GetConfirmationNeed();
    bool MaximumNow();
    bool MinimumNow();
    QVector<double> GetChannelValuesBuffer();
    QVector<double> GetChannelXBuffer();

    void SetConfirmationNeed(bool confirmationstate);
    void SetSignalType(uint16_t newsignaltype);
    void ReadSingleChannelOptionFromFile(int channel);
    void SetChannelName(QString newname);
    void SetLowerLimit(double newsignaltype);
    void SetHigherLimit(double newhigherlimit);
    void SetLowerMeasureLimit(double newlowermeaslimit);
    void SetHigherMeasureLimit(double newhighermeaslimit);
    void SetMeasurePeriod(double newmeasperiod);
    void SetUnitsName(QString newunit);
    void SetState1Value(double newvalue);
    void SetState2Value(double newvalue);
    void SetState1HighMessage(QString newmessage);
    void SetState1LowMessage(QString newmessage);
    void SetState2HighMessage(QString newmessage);
    void SetState2LowMessage(QString newmessage);
    void SetMathEquation(QString newmathstring);
    void SetMathematical(bool newstate);
    void SetCurrentChannelValue(double value);
    void SetDempher(double newdempher);
    void SetDiapason(int newdiapason);
    void SetRegistrationType(int newdregistrationtype);
    void SetNormalColor(QColor newcolor);
    void SetMaximumColor(QColor newmaxcolor);
    void SetMinimumColor(QColor newmincolor);

    bool IsHighState1Setted();
    bool IsLowState1Setted();
    bool IsHighState2Setted();
    bool IsLowState2Setted();
    bool IsChannelMathematical();

    bool HighState1Setted ;
    bool LowState1Setted ;
    bool HighState2Setted;
    bool LowState2Setted ;
    bool MathematicalState;
    int xposition;
    int yposition;
    int w;
    int h;

    // ?????????????????? ???????????????????? ???????????????? ????????????

private:

    uint16_t signaltype;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiod;
    double state1value;
    double state2value;
    double currentvalue;
    double demphervalue;

    QString unitsname;
    QString state1highmessage;
    QString state1lowmessage;
    QString state2highmessage;
    QString state2lowmessage;
    QString channelname;
    QString mathequationstring;

    QColor normalcolor; // ???????? ???????????? ???? ?????????????? ????????
    QColor maximumcolor; // ???????? ???????????? ???? ?????????????? ????????
    QColor minimumcolor; // ???????? ???????????? ???? ?????????????? ??????
    bool needConfirmationchannel;
    int diapason;
    int registrationtype;

    QVector<double> channelvaluesbuffer;
    QVector<double> dempheredvaluesbuffer;
    QVector<double> channelxbuffer;
    QVector<double> channelpercentbuffer;

    QMutex *buffermutex;

public:

    enum SignalType {
        NoMeasure= 0,
        CurrentMeasure= 1,
        VoltageMeasure= 2,
        ResistanceMeasure= 3,
        TermoCoupleMeasure= 4,
        TermoResistanceMeasure= 5,
        DiscretMeasure= 6,
        ImpulseCounterMeasure= 7,
        FrequencyMeasure= 8
    };
    Q_ENUM(SignalType)

    enum SupportedSignals {
        SupportedSignalError= 0,
        SupportedSignalCurrent = 1,
        SupportedSignalVoltage = 2,
        SupportedSignalResistance = 4,
        SupportedSignalTermoCouple = 8,
        SupportedSignalTermoResistance = 16,
        SupportedSignalDiscret = 32,
        SupportedSignalImpulseCounter = 64,
        SupportedSignalFrequency = 128
    };
    Q_ENUM(SupportedSignals)

    enum Status {
        StatusOff = 0,
        StatusOn = 1,
        StatusConfigWait = 2,
        StatusConfigWorking = 3,
        StatusConfigCompleted = 4,
        StatusWaitForUpdateData = 5,
        StatusDataWorking = 6,
        StatusDataCompleted = 7
    };
    Q_ENUM(Status)

    enum ModBusErrors {
        ConnectionError = -9998 ,
        BadCRC = -9999 ,
    };
    Q_ENUM(ModBusErrors)

    enum UartErrors {
        NoResponse = 0x01 ,
        CrcError = 0x02
    };
    Q_ENUM(UartErrors)

    enum DeviceAdress {
        MainDeviceAddress = 0x01 ,
        Board4AIAddress = 0x01 ,
        UniversalChannel1 = 0x01 ,
        UniversalChannel2 = 0x02 ,
        UniversalChannel3 = 0x04 ,
        UniversalChannel4 = 0x08
    };
    Q_ENUM(DeviceAdress)

    enum DeviceType{
        DeviceType4AI = 1,
        DeviceType8RP = 5
    };
    Q_ENUM(DeviceType)

    enum RegisterAdress {
        TemperetureAdress     = 0x0000,
        VoltageAdressHi       = 0x02,
        CurrentAdressHi       = 0x04,
        ResistanceAdressHi    = 0x08,
        FrequencyAdressHi     = 0x10,
        TemperetureAdressLo   = 0x00, // ????????????????, ???? ??????????
        VoltageAdressLo       = 0x00,
        CurrentAdressLo       = 0x00,
        ResistanceAdressLo    = 0x00,
        FrequencyAdressLo     = 0x00,
        ElmetroRelayAddress = 0x01,
        ElmetroChannelAB1Address = 0x00,
        ElmetroChannelAB2Address = 0x02,
        ElmetroChannelAB3Address = 0x04,
        ElmetroChannelAB4Address = 0x06
    };
    Q_ENUM(RegisterAdress)

    enum RegisterCount{
        TemperetureRegLenght = 0x000A,
        VoltageRegCountHi = 0x01,
        CurrentRegCountHi = 0x01,
        ResistanceRegCountHi = 0x01,
        FrequencyRegCountHi = 0x01,
        TemperetureRegCountLo = 0x00,
        VoltageRegCountLo = 0x00,
        CurrentRegCountLo = 0x00,
        ResistanceRegCountLo = 0x00,
        FrequencyRegCountLo = 0x00
    };
    Q_ENUM(RegisterCount)

    enum Function {
        ReadCoils = 0x01,
        ReadDiscreteInputs = 0x02,
        ReadHoldingRegisters = 0x03,
        ReadInputRegisters = 0x04,
        WriteSingleCoil = 0x05,
        WriteSingleRegister = 0x06,
        ReadExceptionStatus = 0x07,
        DiagnosticRequest = 0x08,
        GetCommEventCounter = 0x0B,
        GetCommEventLog = 0x0C,
        WriteMultipleRegisters = 0x10
    };
    Q_ENUM(Function)

    /* ?????????????????? ???????????????????? ?????? ???????????? G00 (???????????????? ???????????? ????????????):
     * ?????? ???????????? ???????? 4AI ?????? ?????????????? ???? ?????????????? 03 (0x03)
     * Read Holding Registers ?? 04 (0x04) Read Input Registers
     * (???????????? ???????????? ??????????????????????).
     * */
    enum G00_Group
    {
        G00Bias = 0,
        DataChannel1 = G00Bias + 2,
        DataChannel2 = G00Bias + 6,
        DataChannel3 = G00Bias + 10,
        DataChannel4 = G00Bias + 14,
        DataChannelLenght = 2
    };
    Q_ENUM(G00_Group)

    /* ???????????? ?? ???????????????????? ?????????????? ?????????????? ????????????????????????????
     * ?? ???????????????????????????? ?????????????? 03 (0x03)
     * Read Holding Registers
     * */

    enum MeasureTypeVoltage{
        Voltage1VoltNoBreakControl = 1,
        Voltage1VoltWithBreakControl = 2,
        Voltage100mVoltNoBreakControl = 3,
        Voltage100mVoltWithBreakControl = 4,
        Voltage5VoltNoBreakControl = 5,
        Voltage5VoltWithBreakControl = 6,
        Voltage10VoltNoBreakControl = 7,
        Voltage10VoltWithBreakControl = 8
    };
    Q_ENUM(MeasureTypeVoltage)


    enum MeasureTypeWireMode{
        Wire3NoBreakControl = 3,
        Wire3WithBreakControl = 4,
        Wire4NoBreakControl = 5,
        Wire4WithBreakControl = 6
    };
    Q_ENUM(MeasureTypeWireMode)

    enum TermoCoupleType{
        R = 1,  //(?????? 13)
        S = 2,  //(?????? 10)
        B = 3,  //(??????)
        J = 4,  //(??????)
        T = 5,  //(????????)
        E = 6,  //(????????)
        K = 7,  //(??????)
        N = 8,  //(??????)
        A1 = 9, //(??????)
        A2 = 10,//(??????)
        A3 = 11,//(??????)
        L = 12, //(??????)
        M = 14  //(??????)
    };
    Q_ENUM(TermoCoupleType)

    enum TermoResistorType{
        Pt1dot3910 = 1,
        Pt1dot3850  = 2,
        Cu1dot4280 = 3,
        Cu1dot4260  = 4,
        Ni1dot6170 = 5,
        m5m3m = 6,
        PP46P = 7
    };
    Q_ENUM(TermoResistorType)

    enum SignalTypeSelect{
        MeasureOff = 0,
        MeasureCurrent = 1,
        MeasureVoltage = 2,
        MeasureResistance = 3,
        MeasureTC = 4,
        MeasureTermoResistance = 5,
        MeasureDiscrette = 6,
        MeasureImpulseCount = 7,
        MeasureImpulseFrequency = 8
    };
    Q_ENUM(SignalTypeSelect)

    enum FilterTypeSelect{
        FilterOff = 0,
        FilterMediana = 1,
        FilterAverage = 2
    };
    Q_ENUM(FilterTypeSelect)

    enum TransmissionFunction{
        LinearTransmission = 0,
        QuadroTransmission = 1,
        SQRTTransmission = 2
    };
    Q_ENUM(TransmissionFunction)

    enum CurrentDiapason{
        Current4_20mA ,
        Current0_20mA ,
        Current0_5mA ,
        Current0_20mA_sqrt ,
        Current4_20mA_sqrt ,
        Current_20_20mA
    };
    Q_ENUM(CurrentDiapason)


    enum VoltageDiapason{
        Voltage0_1V ,
        Voltage0_10V ,
        Voltage0_5V ,
        Voltage1_5V ,
        Voltage150_150mV ,
        Voltage1_1V ,
        Voltage10_10V ,
        Voltage30_30V ,
        Voltage0_1V_sqrt ,
        Voltage0_10V_sqrt ,
        Voltage1_5V_sqrt
    };
    Q_ENUM(VoltageDiapason)
};
#endif // CHANNEL1_H
