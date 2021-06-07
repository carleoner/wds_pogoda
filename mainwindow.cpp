#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wind_arrow.h"
#include <QElapsedTimer>
#include <QDebug>

/*!
 * \file
 * \brief Plik klasy głównego okna
 *
 * Plik źródłowy
 */

/*!
 * \param[in] Rodzic
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QPixmap image(":/images/map.png");
    ui->label_map_big->setPixmap(image.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->stackedWidget->setCurrentWidget(ui->page_1);
    QObject::connect(ui->Button_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    QObject::connect(ui->Button_ros, SIGNAL(clicked()),this,SLOT(page3()));
    QObject::connect(ui->Button_koronowskie, SIGNAL(clicked()),this,SLOT(page4()));

    QObject::connect(ui->lake_icon_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    QObject::connect(ui->lake_icon_ros, SIGNAL(clicked()),this,SLOT(page3()));
    QObject::connect(ui->lake_icon_koronowskie, SIGNAL(clicked()),this,SLOT(page4()));

    QObject::connect(ui->Button_home2, SIGNAL(clicked()),this,SLOT(page1()));
    QObject::connect(ui->Button_home2_2, SIGNAL(clicked()),this,SLOT(page1()));
    QObject::connect(ui->Button_home2_3, SIGNAL(clicked()),this,SLOT(page1()));

}


MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \param[in] Wydarzenie zmiany rozmiaru okna
 */

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    //QWidget *temp = new QWidget();
    //temp = ui->stackedWidget->currentWidget();

    //ui->stackedWidget->setCurrentWidget(ui->page_1);
    ui->lake_icon_zywieckie->setGeometry(width()/2-50,height()/2+100, 45, 45);
    ui->lake_icon_ros->setGeometry(width()/2+80,height()/2-190, 45, 45);
    ui->lake_icon_koronowskie->setGeometry(width()/2-60,height()/2-140, 45, 45);


    QObject::connect(ui->lake_icon_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    QObject::connect(ui->lake_icon_ros, SIGNAL(clicked()),this,SLOT(page3()));
    QObject::connect(ui->lake_icon_koronowskie, SIGNAL(clicked()),this,SLOT(page4()));


    QLabel *label_date = new QLabel(this);

    label_date->setParent(ui->page_1);
    label_date->setGeometry(ui->graphicsView->geometry());
}

/*!
 * \param [in] reply - Odpowiedź z danymi o aktualnej pogodzie
 */

void MainWindow::reload_data(QNetworkReply *reply)
{

    //if there is no errors
    if (!reply->error()) {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &jsonError);

        if(jsonError.error != QJsonParseError::NoError){
            qDebug() << "Error: " << jsonError.errorString();
            //return;
        }

        ///Sprawdza typ odpowiedzi {} (object) / [] (array)
        /// Przekazuje dane do zmiennych z klasy Weather
        //check if the document is an object {} / if [] dosument is an Array
        if (document.isObject())
        {
            QJsonObject obj = document.object();
            QJsonObject tempObject;
            QJsonArray tempArray;
            QJsonValue val;

            if (obj.contains(QStringLiteral("weather")))
            {
                val = obj.value(QStringLiteral("weather"));
                tempArray = val.toArray();
                val = tempArray.at(0);
                tempObject = val.toObject();
                data_weather.description = tempObject.value(QStringLiteral("description")).toString();
                data_weather.icon = tempObject.value("icon").toString();
                qDebug() << data_weather.description;
            }
            if (obj.contains(QStringLiteral("main")))
            {
                val = obj.value(QStringLiteral("main"));
                tempObject = val.toObject();
                data_weather.temp = tempObject.value(QStringLiteral("temp")).toDouble();
                data_weather.temp_min = tempObject.value(QStringLiteral("temp_min")).toDouble();
                data_weather.temp_max = tempObject.value(QStringLiteral("temp_max")).toDouble();
                data_weather.temp_realfeel = tempObject.value(QStringLiteral("feels_like")).toDouble();
                data_weather.pressure = tempObject.value(QStringLiteral("pressure")).toDouble();
                data_weather.humidity = tempObject.value(QStringLiteral("humidity")).toDouble();
                qDebug() << data_weather.temp;
            }
            if (obj.contains(QStringLiteral("visibility")))
            {
                val = obj.value(QStringLiteral("visibility"));
                data_weather.visibility = val.toInt();
            }
            if (obj.contains(QStringLiteral("wind")))
            {
                val = obj.value(QStringLiteral("wind"));
                tempObject = val.toObject();
                data_weather.speed = tempObject.value(QStringLiteral("speed")).toDouble();
                data_weather.gust = tempObject.value(QStringLiteral("gust")).toDouble();
                data_weather.wind_angle_deg = tempObject.value(QStringLiteral("deg")).toInt();
                qDebug() << data_weather.wind_angle_deg;
            }
            if (obj.contains(QStringLiteral("clouds")))
            {
                data_weather.cloudiness =  obj.value(QStringLiteral("clouds")).toObject().value(QStringLiteral("all")).toInt();
            }
            if  (obj.contains(QStringLiteral("rain")))
            {
                val = obj.value(QStringLiteral("rain"));
                tempObject = val.toObject();
                data_weather.rain1h = tempObject.value(QStringLiteral("1h")).toDouble();
                qDebug() << data_weather.rain1h;
                qDebug() << data_weather.wind_angle_deg;
            }
        }
    }
    reply->deleteLater();
}

/*!
 * \param[in] reply - Odpowiedź z danymi o prognozowanej pogodzie
 */

void MainWindow::reload_data_forecast(QNetworkReply *reply)
{
    if (!reply->error()) {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &jsonError);

        if(jsonError.error != QJsonParseError::NoError){
            qDebug() << "Error: " << jsonError.errorString();
        }

        ///check if the document is an object {} / if [] dosument is an Array
        if (document.isObject())
        {
            QJsonObject obj = document.object();
            QJsonObject tempObject;
            QJsonArray tempArray;
            QJsonValue val;

            if (obj.contains(QStringLiteral("hourly")))
            {
                val = obj.value(QStringLiteral("hourly"));
                tempArray = val.toArray();

                for(int i=0;i<6;i++){
                    val = tempArray.at(i);
                    tempObject = val.toObject();
                    data_weather.data_time[i] = tempObject.value(QStringLiteral("dt")).toInt();
                    data_weather.speed_forecast[i] = tempObject.value(QStringLiteral("wind_speed")).toDouble();
                    data_weather.gust_forecast[i] = tempObject.value(QStringLiteral("wind_gust")).toDouble();
                    if  (tempObject.contains(QStringLiteral("rain")))
                    {
                        val = tempObject.value(QStringLiteral("rain"));
                        tempObject = val.toObject();
                        data_weather.rain1h_forecast[i] = tempObject.value(QStringLiteral("1h")).toDouble();
                    }
                    else
                    {
                        data_weather.rain1h_forecast[i] = 0;
                    }
                    qDebug() << data_weather.speed_forecast[i] << data_weather.rain1h_forecast[i] << data_weather.data_time[i];
                }
//                data_weather.rain1h_forecast[0] = 0.15;
//                data_weather.rain1h_forecast[1] = 0.25;
//                data_weather.rain1h_forecast[2] = 0.06;
//                data_weather.rain1h_forecast[3] = 0.21;
//                data_weather.rain1h_forecast[4] = 0.1;
//                data_weather.rain1h_forecast[5] = 0.12;
            }
        }
    }
    reply->deleteLater();
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */
bool MainWindow::check_for_an_update()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url = QUrl(myURL0);
    QNetworkRequest request(url);
    man->get(request);
    return true;
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */

bool MainWindow::check_for_an_update_forecast()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data_forecast);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url = QUrl(myURL00);
    QNetworkRequest request(url);
    man->get(request);
    return true;
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */

bool MainWindow::check_for_an_update1()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url1 = QUrl(myURL1);
    QNetworkRequest request1(url1);
    man->get(request1);
    return true;
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */

bool MainWindow::check_for_an_update_forecast1()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data_forecast);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url = QUrl(myURL11);
    QNetworkRequest request(url);
    man->get(request);
    return true;
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */

bool MainWindow::check_for_an_update2()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url2 = QUrl(myURL2);
    QNetworkRequest request2(url2);
    man->get(request2);
    return true;
}

/*!
 * \retval true - udane
 * \retval false - nieudane
 */

bool MainWindow::check_for_an_update_forecast2()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &MainWindow::reload_data_forecast);
    if (!&QNetworkAccessManager::finished)
        return false;
    const QUrl url = QUrl(myURL22);
    QNetworkRequest request(url);
    man->get(request);
    return true;
}


void MainWindow::page1()
{

    QObject::connect(ui->Button_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}


void MainWindow::page2()
{

    bool ready = check_for_an_update();
    bool ready_forecast = check_for_an_update_forecast();

    page_loading();
    while(ready == false || ready_forecast == false)
        page_loading();

    QObject::connect(ui->Button_home2, SIGNAL(clicked()),this,SLOT(page1()));
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    QPixmap image(":/images/zywieckie.png");
    //ui->label_zywieckie->setPixmap(image.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    page_content(2);
/*
    //cloud
    QPixmap image_cloud(":/images/oie_transparent.png");
    ui->lbl_Clouds->setPixmap(image_cloud.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    animation = new QPropertyAnimation(ui->lbl_Clouds, "geometry");
    animation->setStartValue(ui->lbl_Clouds->geometry());
    animation->start();
*/

}


void MainWindow::page3()
{
    bool ready = check_for_an_update1();
    bool ready_forecast = check_for_an_update_forecast1();

    page_loading();
    while(ready == false || ready_forecast == false)
        page_loading();

    QObject::connect(ui->Button_home2_2, SIGNAL(clicked()),this,SLOT(page1()));
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    QPixmap image(":/images/ros.png");
    //ui->label_zywieckie->setPixmap(image.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    page_content(3);
}



void MainWindow::page4()
{
    bool ready = check_for_an_update2();
    bool ready_forecast = check_for_an_update_forecast2();

    page_loading();
    while(ready == false || ready_forecast == false)
        page_loading();

    QObject::connect(ui->Button_home2_3, SIGNAL(clicked()),this,SLOT(page1()));
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    QPixmap image(":/images/koronowskie.png");
    //ui->label_zywieckie->setPixmap(image.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    page_content(4);
}

/*!
 * Funkcja posiada licznik czasu.
 */

void MainWindow::page_loading()
{
    ui->stackedWidget->setCurrentWidget(ui->page_loading);
    QEventLoop loop;
    QTimer timer;

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    //connect(this, SIGNAL(replayReceived()), &loop, SLOT(quit()));

    timer.start(1000);
    loop.exec();

    if (timer.isActive()) //replay received before timer
    {

    }
    else //timer elapsed, no replay from client
    {

    }
    //scene = new QGraphicsScene();
    //scene->clear();
}

/*!
 * W funkcji znajduje się ustawiwenie napisów
 * oraz wywołanie animacji.
 * \param[in] page_number - identyfikator wyświetlanej strony aplikacji
 */

void MainWindow::page_content(int page_number)
{

    ui->setupUi(this);

    QPixmap image(":/images/map.png");
    ui->label_map_big->setPixmap(image.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->stackedWidget->setCurrentWidget(ui->page_2);
    QObject::connect(ui->Button_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    QObject::connect(ui->Button_ros, SIGNAL(clicked()),this,SLOT(page3()));
    QObject::connect(ui->Button_koronowskie, SIGNAL(clicked()),this,SLOT(page4()));

    QObject::connect(ui->lake_icon_zywieckie, SIGNAL(clicked()),this,SLOT(page2()));
    QObject::connect(ui->lake_icon_ros, SIGNAL(clicked()),this,SLOT(page3()));
    QObject::connect(ui->lake_icon_koronowskie, SIGNAL(clicked()),this,SLOT(page4()));

    QObject::connect(ui->Button_home2, SIGNAL(clicked()),this,SLOT(page1()));
    QObject::connect(ui->Button_home2_2, SIGNAL(clicked()),this,SLOT(page1()));
    QObject::connect(ui->Button_home2_3, SIGNAL(clicked()),this,SLOT(page1()));



//    QDateTime timestamp = QDateTime::currentDateTime();
//    int hour = timestamp.time().hour();
//    qDebug() << hour;

//    quint32 unixtime = data_weather.data_time[1];
//    QDateTime myDateTime;
//    myDateTime.setTime_t(unixtime);
//    int hour = myDateTime.time().hour();
//    qDebug() << hour;





    QLineSeries *series = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();

    for(int i=0;i<6;i++){
        series->append(i, data_weather.speed_forecast[i]);
        series1->append(i, data_weather.gust_forecast[i]);
    }

    // Create chart, add data, show legend, and add axis
    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->addSeries(series);
    chart->addSeries(series1);
    chart->createDefaultAxes();

    // Customize the title font
    QFont font;
    font.setPixelSize(15);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Prognoza wiatru");

    // Change the line color and weight
    QPen pen(QRgb(0x000000));
    pen.setWidth(2);
    series->setPen(pen);
    series->setName("Średnia prędędkość wiatru");
    QPen pen1(QColor( 0xFF, 0xA0, 0x00 ));
    //pen1.setColor("orange");
    pen1.setWidth(2);
    series1->setPen(QPen(Qt::darkGreen, 2));
    series1->setName("Porywistość wiatru");


    chart->setAnimationOptions(QChart::AllAnimations);

    int hours;
    QDateTime myDateTime;
    quint32 unixtime;
    QString s;

    QCategoryAxis *axisX = new QCategoryAxis();
    for(int i=0;i<6;i++){
        unixtime = data_weather.data_time[i];
        myDateTime.setTime_t(unixtime);
        hours = myDateTime.time().hour();
        s = QString::number(hours);
        axisX->append(s,i);
    }
    axisX->setTitleText("Godzina");
    chart->setAxisX(axisX, series);
    chart->setAxisX(axisX, series1);

    //Wyznaczanie maksymalnej wartości osi Y
    double max_val = 0, temp;
    for(int i=0;i<6;i++){
        temp = data_weather.speed_forecast[i];
        if(max_val<temp)
            max_val = temp;
        temp = data_weather.gust_forecast[i];
        if(max_val<temp)
            max_val = temp;
    }
    qDebug() << max_val;

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Prędkość [m/s]");
    axisY->setLabelFormat("%g");
    axisY->setRange(0,max_val);
    //axisY->setBase(8.0);
    //axisY->setMinorTickCount(-1);
    chart->addAxis(axisY, Qt::AlignLeft);
    //series->attachAxis(axisY);
    chart->setAxisY(axisY, series);
    chart->setAxisY(axisY, series1);

    // Used to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);
    chartView->setFixedSize(ui->horizontalFrame->size());









    //Wykres z opadami

    QBarSet *set0 = new QBarSet("Opady [mm]");

    for(int i=0;i<6;i++){
        *set0 << data_weather.rain1h_forecast[i];
    }

    QBarSeries *series_rain = new QBarSeries();

    series_rain->append(set0);

    QChart *chart1 = new QChart();
    chart1->addSeries(series_rain);
    chart1->legend()->setVisible(false);
    chart1->legend()->setAlignment(Qt::AlignBottom);

    chart1->setTitle("Prognoza opadów");
    chart1->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    //QCategoryAxis *axisX1 = new QCategoryAxis();
    for(int i=0;i<6;i++){
        unixtime = data_weather.data_time[i];
        myDateTime.setTime_t(unixtime);
        hours = myDateTime.time().hour();
        s = QString::number(hours);
        categories << s;
        //axisX1->append(s,i);
    }
    //chart->setAxisX(axisX1, series_rain);

    QBarCategoryAxis *axisX1 = new QBarCategoryAxis();
    axisX1->setTitleText("Godzina");
    axisX1->append(categories);
    chart1->createDefaultAxes();

    chart1->setAxisX(axisX1, series_rain);

    //stacked bar chart
    //chart->setAxisY(axisX1, series_rain);


    //wyznaczanie maksymalnej wartości osi Y
    double max_val1 = 0;
    double temp1 = 0;
    for(int i=0;i<6;i++){
        temp1 = data_weather.rain1h_forecast[i];
        if(max_val1<temp1)
            max_val1 = temp1;
    }
    qDebug() << max_val1;

    QValueAxis *axisY1 = new QValueAxis();
    axisY1->setTitleText("Opady [mm]");
    axisY1->setLabelFormat("%g");
    axisY1->setRange(0,max_val1);
    axisY1->setMin(0);
    //axisY->setBase(8.0);
    //axisY->setMinorTickCount(-1);
    chart1->addAxis(axisY1, Qt::AlignLeft);
    //series->attachAxis(axisY);
    chart1->setAxisY(axisY1, series_rain);



    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setRenderHint(QPainter::Antialiasing);
    chartView1->setParent(ui->horizontalFrame_2);
    chartView1->setFixedSize(ui->horizontalFrame_2->size());




    //ustawienie napisóW
    ui->label_deg->setText("Opis: ");
    ui->label_temp->setText("Temperatura: ");
    ui->label_speed->setText("Pręd. wiatru: ");
    ui->label_cloudiness->setText("Zachmurzenie: ");
    ui->label_humidity->setText("Wilgotność: ");
    ui->label_pressure->setText("Ciśnienie: ");
    ui->label_gust->setText("Porywistość: ");
    ui->label_visibility->setText("Widoczność: ");

    //wypełnienie napisów danymi
    ui->label_cloudiness_2->setText(QString::number(data_weather.cloudiness) + "%");
    ui->label_cloudiness_2->setStyleSheet("QLabel { background-color : white; color : darkBlue; }");
    ui->label_humidity_2->setText(QString::number(data_weather.humidity) + "%");
    ui->label_humidity_2->setStyleSheet("QLabel { background-color : white; color : magenta; }");
    ui->label_pressure_2->setText(QString::number(data_weather.pressure) + " hPa");
    ui->label_pressure_2->setStyleSheet("QLabel { background-color : white; color : gray; }");
    ui->label_visibility_2->setText(QString::number(data_weather.visibility) + " m");
    ui->label_visibility_2->setStyleSheet("QLabel { background-color : white; color : green; }");
    ui->label_deg_2->setText(QString(data_weather.description + ""));
    ui->label_deg_2->setStyleSheet("QLabel { background-color : white; color : blue; }");
    ui->label_temp_2->setText(QString::number(data_weather.temp_realfeel) + "°C");
    ui->label_temp_2->setStyleSheet("QLabel { background-color : white; color : darkCyan; }");


    //kolorowanie pola prędkośći wiatru w zależności od prędkości
    ui->label_speed_2->setText(QString::number(data_weather.speed) + " m/s");
    ui->label_speed_2->setStyleSheet("QLabel { color : black;}");

    if(data_weather.speed<2){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(70,253,136)}");
    }
    else if (data_weather.speed>2 && data_weather.speed<3){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(150,253,136)}");
    }
    else if (data_weather.speed>3 && data_weather.speed<4){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(200,253,136)}");
    }
    else if (data_weather.speed>4 && data_weather.speed<6){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(250,253,136)}");
    }
    else if (data_weather.speed>6 && data_weather.speed<8){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(253,200,136)}");
    }
    else if (data_weather.speed>8 && data_weather.speed<10){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(253,150,136)}");
    }
    else if (data_weather.speed>10){
        ui->label_speed_2->setStyleSheet("QLabel { background-color : rgb(253,136,136)}");
    }


    //kolorowanie pola porywistości wiatru w zależności od prędkości
    ui->label_gust_2->setText(QString::number(data_weather.gust) + " m/s");
    ui->label_gust_2->setStyleSheet("QLabel { color : black;}");

    if(data_weather.gust<2){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(70,253,136)}");
    }
    else if (data_weather.gust>2 && data_weather.gust<3){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(150,253,136)}");
    }
    else if (data_weather.gust>3 && data_weather.gust<4){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(200,253,136)}");
    }
    else if (data_weather.gust>4 && data_weather.gust<6){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(250,253,136)}");
    }
    else if (data_weather.gust>6 && data_weather.gust<8){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(253,200,136)}");
    }
    else if (data_weather.gust>8 && data_weather.gust<10){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(253,150,136)}");
    }
    else if (data_weather.gust>10){
        ui->label_gust_2->setStyleSheet("QLabel { background-color : rgb(253,136,136)}");
    }


    //QDateTime now = QDateTime::currentDateTime();
    QDate cd = QDate::currentDate();
    //ui->label_date_loca->setText(cd.toString("dd.MM.yyyy"));


    //ui->lcdNumber->display(data_weather.gust);

    //ustawienie ikony pogody
    if(data_weather.icon == "01d"){
        QPixmap image(":/images/01d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "01n"){
        QPixmap image(":/images/01n.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "02d"){
        QPixmap image(":/images/02d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "02n"){
        QPixmap image(":/images/02n.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "03d" || data_weather.icon == "03n"){
        QPixmap image(":/images/03d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "04d" || data_weather.icon == "04n"){
        QPixmap image(":/images/04d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "09d" || data_weather.icon == "09n"){
        QPixmap image(":/images/09d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "10d"){
        QPixmap image(":/images/10d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "10n"){
        QPixmap image(":/images/10n.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "11d" || data_weather.icon == "11n"){
        QPixmap image(":/images/11d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "13d" || data_weather.icon == "13n"){
        QPixmap image(":/images/13d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if (data_weather.icon == "50d" || data_weather.icon == "50n"){
        QPixmap image(":/images/50d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else{
        QPixmap image(":/images/02d.png");
        ui->label_icon->setPixmap(image.scaled(ui->label_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }


    //ustawnienie sceny
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //scene->setSceneRect(20,130,200,200);


    //poprawne
    //scene->setSceneRect(ui->label_zywieckie->geometry());





    //scene->setSceneRect(this->frameGeometry());

    //ustawienie mapy danego zbiornika
    if(page_number == 2){
        ui->graphicsView->setStyleSheet("border-image: url(:/images/zywieckie.png);");
        //QPixmap images(":/images/zywieckie.png");
        //ui->label_zye->setPixmap(images.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if(page_number == 3){
        ui->graphicsView->setStyleSheet("border-image: url(:/images/ros.png);");
        //QPixmap images(":/images/ros.png");
        //ui->label_zywieckie->setPixmap(images.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else if(page_number == 4){
        ui->graphicsView->setStyleSheet("border-image: url(:/images/koronowskie.png);");
        //QPixmap images(":/images/koronowskie.png");
        //ui->label_zywieckie->setPixmap(images.scaled(ui->label_map_big->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
        qDebug() << "error content";

    //ui->graphicsView->setStyleSheet("background: transparent");
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //utworzenie obiektów do animacji
    int CloudCount = data_weather.cloudiness + 20;
    for(int i=0; i<CloudCount; i++)
    {
        Wind_arrow *arrow = new Wind_arrow(data_weather.speed,data_weather.wind_angle_deg);
        scene->addItem(arrow);
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(100);
}


