#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "stackedoptions.h"
#include "qglobal.h"
#include "defines.h"

int a=0;int b=0;

int color1rgbnormal[]={0xCF, 0xF7, 0x00};
int color2rgbnormal[]={0x00, 0xAE, 0x68};
int color3rgbnormal[]={0xFF, 0x4C, 0x00};
int color4rgbnormal[]={0xA1, 0x01, 0xA6};

QColor ChannelColorHighState = QColor(0xFF,0x00,0x00);
QColor ChannelColorLowState  = QColor(0xFF,0xCA,0x00);

QColor Channel1Color = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2Color = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3Color = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4Color = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor ChannelColorNormal = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2ColorNormal = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3ColorNormal = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4ColorNormal = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;

int xyi;

void MainWindow::AddValuesToBuffer()
{

    startWorkSignal();
    X_Coordinates.append(b);
    X_Coordinates_archive.append(b);

    Y_coordinates_Chanel_1.append(UartDriver::channelinputbuffer[0]);
    Y_coordinates_Chanel_2.append(UartDriver::channelinputbuffer[1]);
    Y_coordinates_Chanel_3.append(UartDriver::channelinputbuffer[2]);
    Y_coordinates_Chanel_4.append(UartDriver::channelinputbuffer[3]);

    Y_coordinates_Chanel_1_archive.append(UartDriver::channelinputbuffer[0]);
    Y_coordinates_Chanel_2_archive.append(UartDriver::channelinputbuffer[1]);
    Y_coordinates_Chanel_3_archive.append(UartDriver::channelinputbuffer[2]);
    Y_coordinates_Chanel_4_archive.append(UartDriver::channelinputbuffer[3]);

    while (X_Coordinates.length()>300)
    {
        X_Coordinates.removeFirst();Y_coordinates_Chanel_1.removeFirst();Y_coordinates_Chanel_2.removeFirst();Y_coordinates_Chanel_3.removeFirst();Y_coordinates_Chanel_4.removeFirst();
    }

    int tickstep = GetTickStep();

    if (b%tickstep==0)
    {
        LabelsCorrect();
    }

    b++;
    stopWorkSignal();
}

void MainWindow::UpdateGraphics()
{
    needupdatePainter = 1;

    //    StackedOptions::SetCurrentDisplayParametr(StackedOptions::Bars ); // Bars

    switch( StackedOptions::GetCurrentDisplayParametr() )
    {
    case Options::Trends:
        GrafsUpdateTrends();break;
    case Options::TrendsCyfra:
        GrafsUpdateTrends();break;
    case Options::Bars :
        GrafsUpdateBars();break;
    case Options::BarsCyfra :
        GrafsUpdateBars();break;
    case Options::TrendsBars:
        GrafsUpdateTrendsAndBars();break;
    case Options::TrendsCyfraBars:
        GrafsUpdateTrendsAndBars();break;
    case Options::Polar:
        GrafsUpdateNone();break;
    case Options::Cyfra:
        GrafsUpdateNone();break;
    default:
        break;
    }
}

void MainWindow::GrafsUpdateTrendsAndBars()
{
    while (X_Coordinates.length()>300)
    {
        X_Coordinates.remove(0);Y_coordinates_Chanel_1.remove(0);Y_coordinates_Chanel_2.remove(0);Y_coordinates_Chanel_3.remove(0);Y_coordinates_Chanel_4.remove(0);
    }

    ui->customPlot->xAxis->setRange(b-GetXRange(), b+GetXRange());
    ui->customPlot->clearGraphs();


    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(GetChannel1Color());

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(GetChannel2Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(GetChannel3Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(GetChannel4Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    /// bars

    QVector<double> x1,x2,x3,x4;
    QVector<double> y1,y2,y3,y4;

    QVector<double> x1lim,x2lim,x3lim,x4lim;
    QVector<double> y1max,y2max,y3max,y4max;
    QVector<double> y1min,y2min,y3min,y4min;

    int lastindex = X_Coordinates.at(X_Coordinates.length()-1);

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_1);
    double chan2higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_2);
    double chan3higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_3);
    double chan4higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_4);

    double chan1lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_1);
    double chan2lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_2);
    double chan3lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_3);
    double chan4lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_4);

    y1max.append(chan1higherstate);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);

    y2max.append(chan2higherstate);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);

    y3max.append(chan3higherstate);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);

    y4max.append(chan4higherstate);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);


    y1min.append(chan1lowerstate);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);

    y2min.append(chan2lowerstate);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);


    y3min.append(chan3lowerstate);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);

    y4min.append(chan4lowerstate);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);


    x1.append(430-300+lastindex);
    x1.append(460-300+lastindex);
    x2.append(470-300+lastindex);
    x2.append(500-300+lastindex);
    x3.append(510-300+lastindex);
    x3.append(540-300+lastindex);
    x4.append(550-300+lastindex);
    x4.append(580-300+lastindex);

    x1lim.append(x1.at(0));
    x1lim.append(x1.at(1));
    x1lim.append(x1.at(0));
    x1lim.append(x1.at(0));
    x1lim.append(x1.at(1));
    x1lim.append(x1.at(1));

    x2lim.append(x2.at(0));
    x2lim.append(x2.at(1));
    x2lim.append(x2.at(0));
    x2lim.append(x2.at(0));
    x2lim.append(x2.at(1));
    x2lim.append(x2.at(1));

    x3lim.append(x3.at(0));
    x3lim.append(x3.at(1));
    x3lim.append(x3.at(0));
    x3lim.append(x3.at(0));
    x3lim.append(x3.at(1));
    x3lim.append(x3.at(1));

    x4lim.append(x4.at(0));
    x4lim.append(x4.at(1));
    x4lim.append(x4.at(0));
    x4lim.append(x4.at(0));
    x4lim.append(x4.at(1));
    x4lim.append(x4.at(1));

    y1.append(channel1object.GetCurrentChannelValue());
    y1.append(channel1object.GetCurrentChannelValue());
    y2.append(channel2object.GetCurrentChannelValue());
    y2.append(channel2object.GetCurrentChannelValue());
    y3.append(channel3object.GetCurrentChannelValue());
    y3.append(channel3object.GetCurrentChannelValue());
    y4.append(channel4object.GetCurrentChannelValue());
    y4.append(channel4object.GetCurrentChannelValue());

    graphPen.setWidth(GraphWidthinPixels);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1, y1);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel1Color()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2, y2);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel2Color()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3, y3);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel3Color()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4, y4);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel4Color()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    // рисуем границы каналов каждого барграфа

    QPen dottedpen = QPen(Qt::red, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4max);
    ui->customPlot->graph()->setPen(dottedpen);


    dottedpen = QPen(Qt::green, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);

    // add the arrows:

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);

    QList<int> arrowsendcoords;

    arrowsendcoords.append(x1.at(0));
    arrowsendcoords.append(x2.at(0));
    arrowsendcoords.append(x3.at(0));
    arrowsendcoords.append(x4.at(0));

    // рисуем стрелки для каждой уставки

    int barindex = 0 ;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList)
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState1Value() );
        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState1Value() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState2Value() );
        arrow2->end->setCoords(arrowsendcoords.at(barindex++)-1,Chanel->GetState2Value() );
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow2);


        QPointF Label1PixPoint = arrow->start->pixelPoint();
        QPointF Label2PixPoint = arrow2->start->pixelPoint();

        Label1PixPoint.setY(Label1PixPoint.y() - 20);
        Label2PixPoint.setY(Label2PixPoint.y() - 20);

        // add the text label at the top limit:
        QCPItemText *textLabelHi = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelHi);
        textLabelHi->position->setPixelPoint(Label1PixPoint);
        textLabelHi->setText(QString::number(Chanel->GetState1Value() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit

        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        textLabelLo->setText(QString::number(Chanel->GetState2Value() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));
    }


    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrow:

    if (ui->arrowscheckBox->checkState()) // if it s needed
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1object);
        ChannelsObjectsList.append(&channel2object);
        ChannelsObjectsList.append(&channel3object);
        ChannelsObjectsList.append(&channel4object);

        QList<int> arrowsendcoords;

        arrowsendcoords.append(x1.at(0));
        arrowsendcoords.append(x2.at(0));
        arrowsendcoords.append(x3.at(0));
        arrowsendcoords.append(x4.at(0));

        // рисуем стрелки для каждой уставки

        int index = 0 ;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
            //    arrow->start->setCoords(400,200);

            int ystart = 150;
            int xstart = 600;

            arrow->start->setPixelPoint(QPointF(xstart, ystart+100*index));
            arrow->end->setCoords(b, Chanel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(Chanel->GetCurrentColor(),1,  Qt::DashLine));
            ui->customPlot->addItem(arrow);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(arrow->start->pixelPoint());
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 8, QFont::Bold));
            NameLabel->setColor(Chanel->GetCurrentColor());

            ++index;
        }
    }

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateTrends()
{
    ui->customPlot->xAxis->setRange(b-GetXRange(), b+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);

    //    // add the text label at the top:
    //    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    //    ui->customPlot->addItem(textLabel);
    //    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    //    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    //    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    //    textLabel->setText("Text Item Demo");
    //    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    //    textLabel->setPen(QPen(Qt::black)); // show black border around text

    // add the helper arrow:

    if (ui->arrowscheckBox->checkState())
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1object);
        ChannelsObjectsList.append(&channel2object);
        ChannelsObjectsList.append(&channel3object);
        ChannelsObjectsList.append(&channel4object);

        QList<int> arrowsendcoords;


        // рисуем стрелки для каждой уставки

        int index = 0 ;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
            //    arrow->start->setCoords(400,200);

            int ystart = 150;
            int xstart = 600;

            arrow->start->setPixelPoint(QPointF(xstart, ystart+100*index));
            arrow->end->setCoords(b, Chanel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(Chanel->GetCurrentColor(),1,  Qt::DashLine));
            ui->customPlot->addItem(arrow);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(arrow->start->pixelPoint());
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 8, QFont::Bold));
            NameLabel->setColor(Chanel->GetCurrentColor());

            ++index;
        }

    }

    graphPen.setWidth(GraphWidthinPixels);
    //    graphPen.set
    graphPen.setColor(Channel1Color);

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(Channel2Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(Channel3Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(Channel4Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);


    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);

    // авто масшабирование
    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->rescaleAxes();
    }
    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateNone()
{
    ui->MessagesWidget->update();

    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();

    //ui->customPlot->clearMask();
    //ui->customPlot->clearFocus();
    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateBars()
{
    QVector<double> Bar1_X_Coord,Bar2_X_Coord,Bar3_X_Coord,Bar4_X_Coord;
    QVector<double> Bar1_Y_Coord,Bar2_Y_Coord,Bar3_Y_Coord,Bar4_Y_Coord;

    Bar1_X_Coord.append(15);
    Bar1_X_Coord.append(25);
    Bar2_X_Coord.append(35);
    Bar2_X_Coord.append(45);
    Bar3_X_Coord.append(55);
    Bar3_X_Coord.append(65);
    Bar4_X_Coord.append(75);
    Bar4_X_Coord.append(85);

    Bar1_Y_Coord.append(UartDriver::channelinputbuffer[0]);
    Bar1_Y_Coord.append(UartDriver::channelinputbuffer[0]);
    Bar2_Y_Coord.append(UartDriver::channelinputbuffer[1]);
    Bar2_Y_Coord.append(UartDriver::channelinputbuffer[1]);
    Bar3_Y_Coord.append(UartDriver::channelinputbuffer[2]);
    Bar3_Y_Coord.append(UartDriver::channelinputbuffer[2]);
    Bar4_Y_Coord.append(UartDriver::channelinputbuffer[3]);
    Bar4_Y_Coord.append(UartDriver::channelinputbuffer[3]);

    QVector<double> x1lim,x2lim,x3lim,x4lim;
    QVector<double> y1max,y2max,y3max,y4max;
    QVector<double> y1min,y2min,y3min,y4min;

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_1);
    double chan2higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_2);
    double chan3higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_3);
    double chan4higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_4);

    double chan1lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_1);
    double chan2lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_2);
    double chan3lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_3);
    double chan4lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_4);

    y1max.append(chan1higherstate);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);

    y2max.append(chan2higherstate);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);


    y3max.append(chan3higherstate);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);

    y4max.append(chan4higherstate);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);


    y1min.append(chan1lowerstate);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);

    y2min.append(chan2lowerstate);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);


    y3min.append(chan3lowerstate);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);

    y4min.append(chan4lowerstate);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);


    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(1));
    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(1));
    x1lim.append(Bar1_X_Coord.at(1));

    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(1));
    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(1));
    x2lim.append(Bar2_X_Coord.at(1));

    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(1));
    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(1));
    x3lim.append(Bar3_X_Coord.at(1));

    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(1));
    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(1));
    x4lim.append(Bar4_X_Coord.at(1));


    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();

    ui->customPlot->xAxis->setRange(0, 100);
    graphPen.setWidth(GraphWidthinPixels);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("Bargraf");

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar1_X_Coord, Bar1_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel1Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel1Color );
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar2_X_Coord, Bar2_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel2Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel2Color);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar3_X_Coord, Bar3_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel3Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel3Color);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar4_X_Coord, Bar4_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel4Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel4Color);
    ui->customPlot->graph()->setPen(graphPen);

    LabelsBar.clear();
    LabelsBar.append("0");LabelsBar.append(channel1object.GetChannelName());LabelsBar.append(channel2object.GetChannelName());LabelsBar.append(channel3object.GetChannelName());LabelsBar.append(channel4object.GetChannelName());

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(20); // 60 secs btw timestamp
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(LabelsBar);

    // рисуем границы каналов каждого барграфа

    QPen dottedpen = QPen(Qt::red, 1, Qt::DashLine);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4max);
    ui->customPlot->graph()->setPen(dottedpen);

    dottedpen = QPen(Qt::green, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);

    // add the arrows:

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);

    QList<int> arrowsendcoords;

    arrowsendcoords.append(Bar1_X_Coord.at(0));
    arrowsendcoords.append(Bar2_X_Coord.at(0));
    arrowsendcoords.append(Bar3_X_Coord.at(0));
    arrowsendcoords.append(Bar4_X_Coord.at(0));

    // рисуем стрелки для каждой уставки

    int barindex = 0 ;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList)
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->setPen(QPen(Qt::red, 3, Qt::SolidLine));
        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState1Value() );
        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState1Value() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 3, Qt::SolidLine));
        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState2Value() );
        arrow2->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState2Value() );
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow2);


        QPointF Label1PixPoint = arrow->start->pixelPoint();
        QPointF Label2PixPoint = arrow2->start->pixelPoint();

        Label1PixPoint.setY(Label1PixPoint.y() - 20);
        Label2PixPoint.setY(Label2PixPoint.y() - 20);

        // add the text label at the top limit:
        QCPItemText *textLabelHi = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelHi);
        textLabelHi->position->setPixelPoint(Label1PixPoint);
        textLabelHi->setText(QString::number(Chanel->GetState1Value() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit

        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        textLabelLo->setText(QString::number(Chanel->GetState2Value() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));

        ++barindex;
    }




    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }

    ui->customPlot->replot();

    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::UpdateChannel1Slot()
{
    UartDriver::needtoupdatechannel[0] = 1;
    int period = channel1object.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
    channel1object.SetCurrentChannelValue(UartDriver::channelinputbuffer[0]);
    CheckState(channel1object);
}

void MainWindow::UpdateChannel2Slot()
{
    UartDriver::needtoupdatechannel[1] = 1;
    int period = channel2object.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
    channel2object.SetCurrentChannelValue(UartDriver::channelinputbuffer[1]);
    //    qDebug()<<UartDriver::channelinputbuffer[1];
    CheckState(channel2object);
}

void MainWindow::UpdateChannel3Slot()
{
    UartDriver::needtoupdatechannel[2] = 1;
    int period = channel3object.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
    channel3object.SetCurrentChannelValue(UartDriver::channelinputbuffer[2]);
    CheckState(channel3object);
}

void MainWindow::UpdateChannel4Slot()
{
    UartDriver::needtoupdatechannel[3] = 1;
    int period = channel4object.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
    channel4object.SetCurrentChannelValue(UartDriver::channelinputbuffer[3]);
    CheckState(channel4object);
}
