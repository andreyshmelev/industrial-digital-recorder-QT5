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
#include <QtMath>

int a=0;
int b=0;
int color1rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color2rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color3rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color4rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color5rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};

QVector<double> xx1;
QVector<double> yy1;

QVector<double> xx2;
QVector<double> yy2;

QVector<double> xx3;
QVector<double> yy3;

QVector<double> xx4;
QVector<double> yy4;

QVector<double> xx5;
QVector<double> yy5;
/*
QVector<double> xx6;
QVector<double> yy6;

QVector<double> xx7;
QVector<double> yy7;

QVector<double> xx8;
QVector<double> yy8;

QVector<double> xx9;
QVector<double> yy9;

QVector<double> xx10;
QVector<double> yy10;
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("/usr/inc/logo.jpg");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(updategraph()));

    timer->start(1000);
    timer2->start(500);
    timer3->start(100);

    ui->customPlot->xAxis->setRange(-8, 200);
    ui->customPlot->yAxis->setRange(-5, 100);

/*
    int n = 50; // number of points in graph
    double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
    double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
    double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; i++)
    {
        x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
        y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
    }

    ui->customPlot->xAxis->setRange(-8, 200);
    ui->customPlot->yAxis->setRange(-5, 100);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
        ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dial_sliderMoved(int position)
{
    ui->lcdNumber->display(ui->dial->value());

}

void MainWindow::on_lcdNumber_overflow()
{

}

void MainWindow::on_dial_actionTriggered(int action)
{

}

void MainWindow::updateCaption()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->textEdit_2->setText(local.toString());

}


void MainWindow::updatepicture()
{
    //QPixmap ship("C:/Users/aashmele/untitled2/logo.jpg");
/*
    QPixmap ship("/usr/inc/logo.jpg");
    QPixmap rotate(ship.size()) ;
    QPainter p(&rotate);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotate.size().width()/2, rotate.size().height()/2);
    p.rotate(a+(ui->dial->value()-45)*2);
    p.translate(-rotate.size().width()/2, -rotate.size().height()/2);
    p.drawPixmap(0,0,ship);
    p.end();
    ui->label->setPixmap(rotate);
    a++;*/
    QPen graphPen;

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
//    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx1, yy1);


    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));
//    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);


    ui->customPlot->addGraph();
//    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx2, yy2);
    graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
//    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);



    ui->customPlot->addGraph();
//    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx3, yy3);
    graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
//    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);



    ui->customPlot->addGraph();
//    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx4, yy4);
    graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
//    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);



    ui->customPlot->addGraph();
//    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx5, yy5);
    graphPen.setColor(QColor(color5rgb[0],color5rgb[1],color5rgb[2]));
//    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);




    ui->customPlot->replot();
}

void MainWindow::updategraph()
{
    int high=5;
    int low=1;

    xx1.append(b);
    yy1.append(ui->dial_2->value()+qrand() % ((high + 1) - low) + low + 0);


    xx2.append(b);
    yy2.append(ui->dial_3->value()+qrand() % ((high + 1) - low) + low + 10);

    xx3.append(b);
    yy3.append(ui->dial_4->value()+qrand() % ((high + 1) - low) + low + 15);

    xx4.append(b);
    yy4.append(ui->dial_5->value()+qrand() % ((high + 1) - low) + low + 20);

    xx5.append(b);
    yy5.append(ui->dial_6->value()+qrand() % ((high + 1) - low) + low + 25);

//    xx6.append(b);
//    yy6.append(ui->dial_4->value()+qrand() % ((high + 1) - low) + low + 30);



    ///////////////
    b++;

    if (b==200)
    {
        b=0;
        xx1.clear();
        yy1.clear();
        xx2.clear();
        yy2.clear();
        xx3.clear();
        yy3.clear();
        xx4.clear();
        yy4.clear();
        xx5.clear();
        yy5.clear();

        ui->customPlot->clearGraphs();
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{/*
    QPainter painter(this);

    QPen pen1(Qt::black);
    pen1.setWidth(6);

painter.rotate(-45);
    QRect rec(20,60,20,60);
    painter.setPen(pen1);
    painter.drawRect(rec);*/

}

void MainWindow::on_pushButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushButton_2_clicked()
{


    ui->customPlot->clearGraphs();

    int n = 50; // number of points in graph
    double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
    double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
    double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; i++)
    {
        x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
        y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
        ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();




    ///////////////
    QVector<double> x1(200), y1(200);
    QVector<double> x2(201), y2(201);
    //int tre=a%100;

    for (int i=0; i<201; i++)
    {
        x2[i] = i ; // x goes from -1 to 1
        y2[i] = x2[i]/2; // let's plot a quadratic function
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    //    ui->customPlot->graph()->setData({10,20,22,25,27,28,29,30,31,90}, {50,15,30,20,10,90,80,70,60,50});
    //    ui->customPlot->graph()->setData({10,20,30,40,50,60,70,80,90,100},y1);// {50,15,30,20,10,90,80,70,60,50});
    ui->customPlot->graph()->setData(x2, y2);
    //ui->customPlot->graph()->addData(1,10);

    ui->customPlot->replot();
    ///////////////
    b++;
    if(b==200)
    {
        b=0;
        ui->customPlot->clearGraphs();

    }
}

void MainWindow::on_pushButton_2_pressed()
{
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QFile filedir("/sys/class/gpio/gpio69/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio69/value");

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    if (ui->checkBox->checkState())
    {
        out << "1";
    }

    else
    {
        out << "0";

        //qDebug() << ui->checkBox->checkState();
    }
    file.close();
}
