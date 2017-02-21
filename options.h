#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

#include <mainwindow.h>

#include <channel1.h>

namespace Ui {
class Options;

}

class Options : public QDialog
{
    Q_OBJECT

    ChannelOptions options1;
    ChannelOptions options2;
    ChannelOptions options3;
    ChannelOptions options4;

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

    static QString calibrationprm;
    static QString olderprop;
    void setmaxmessageslimit(double n);
    double getmaxmessageslimit();

public slots:

    void WriteOptionsToFile();
    void WriteSystemOptionsToFile();
    bool eventFilter(QObject *object, QEvent *event);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void Channel1TypeChange();

    void Channel2TypeChange();

    void Channel3TypeChange();

    void Channel4TypeChange();

    void applynewsettings();

    void applysettingstoUI();

    void customizeUI();

    void readoptionsfromfile();

    void readsystemoptionsfromfile();

    QString GetSignalUnits();

    void on_UnitsChannel_1_editingFinished();

    void on_pushButton_3_clicked();

    void on_tabWidget_2_tabBarClicked(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:

    static double maxmessageslimit;


    Ui::Options *ui;
};

#endif // OPTIONS_H
