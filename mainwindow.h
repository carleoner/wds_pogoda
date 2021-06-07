#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*!
 * \file
 * \brief W pliku znajduje się definicja klasy głównego okna oraz definicja klasy Weather.
 *
 * Plik zawiera definicję klasy głównego okna,
 * która obsługuje wysłanie zapytania, odebranie go i
 * przerobienie. Znajduje się również funkcja, która
 * obsługuje zawartość każdej ze stron.
 * W klasie Weather,
 * znajdują się informacje o aktualnej i prognozowanej pogodzie.
 * Są to zmienne, które przechowują dane
 * odebrane z internetu w jednej z funkcji reload.
 *
 */

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDebug>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>
#include <QDateTime>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QLegend>
#include <QCategoryAxis>
#include <QBarSeries>
#include <QBarSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief Klasa porzechowująca informacje o pogodzie
 *
 * Zawiera m.in. informacje
 * o temperaturze, opadach czy sile wiatru.
 */

class Weather
{
public:
    /*!
     * \brief Opis pogody
     */
    QString description;
    /*!
     * \brief Ikona ilustrująca pogodę
     */
    QString icon;
    /*!
     * \brief Temperatura, wyrażana w stopniach Celcjusza
     */
    double temp;
    /*!
     * \brief Temperatura minimalna, wyrażana w stopniach Celcjusza
     */
    double temp_min;
    /*!
     * \brief Temperatura maksymalna, wyrażana w stopniach Celcjusza
     */
    double temp_max;
    /*!
     * \brief Temperatura odczuwalna, wyrażana w stopniach Celcjusza
     */
    double temp_realfeel;
    /*!
     * \brief Ciśnienie, wyrażane w [hPa]
     */
    double pressure;
    /*!
     * \brief Wilgotność, wyrażana w [%]
     */
    double humidity;
    /*!
     * \brief Opady deszczu w ciągu najbliższej godziny, wyrażane w [mm]
     */
    double rain1h;

    //for wind and clouds
    /*!
     * \brief Szybkość wiatru, wyrażana w [m/s]
     */
    double speed;
    /*!
     * \brief Porywistość wiatru, wyrażana w [m/s]
     */
    double gust; //gust - rapid wind rush
    /*!
     * \brief Tablica przechowująca prognozowane wartości szybkości wiatru, wyrażane w [m/s]
     */
    double speed_forecast[20];
    /*!
     * \brief Tablica przechowująca prognozowane wartości porywistości wiatru, wyrażane w [m/s]
     */
    double gust_forecast[20];
    /*!
     * \brief Tablica przechowująca prognozowane wartości opadu deszczu, wyrażane w [mm]
     */
    double rain1h_forecast[20];
    /*!
     * \brief Tablica przechowująca czas danych prognozowanych
     */
    quint32 data_time[20];
    /*!
     * \brief Kąt kierunku wiatru, wyrażany w stopniach
     */
    int wind_angle_deg;
    /*!
     * \brief Zachmurzenie, wyrażane w [%]
     */
    int cloudiness;
    /*!
     * \brief Widoczność, wyrażana w [m]
     */
    int visibility;
};


/*!
 * \brief Klasa zawiera informacje o linkach do wysłania zapytań do strony: openweathermap.org
 *
 * W klasie sa definicje funkcji odpowiedzialne
 * za wysłanie zapytania, odebranie go i
 * przerobienie. Posiada też definicje każdej ze stron
 * oraz definicję funkcji określającej zawartość określonej strony.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Inicjalizuje pojęcie głównego okna
     *
     * Ustawia ui, podłącza przyciski, dodaje obraz
     * na główny ekran
     */
    MainWindow(QWidget *parent = nullptr);

    /*!
     * \brief Metoda wywoływana przed usunięciem obiektu
     */
    ~MainWindow();

    /*!
     * \brief Ustala położenie elementów na wypadek zmiany rozmiaru ekranu.
     */
    void resizeEvent(QResizeEvent* event);

private:

    Weather data_weather;



    // Linki do miejsc [aktualna pogoda]

    //Current:
    //Jezioro Zywieckie, Żywiec
    /*!
     * \brief Link do aktualnej pogody dla jeziora Żywieckiego
     */
    QString myURL0 = "http://api.openweathermap.org/data/2.5/weather?q=%C5%BBywiec&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";
    //Jezioro Roś, Pisz
    /*!
     * \brief Link do aktualnej pogody dla jeziora Roś
     */
    QString myURL1 = "http://api.openweathermap.org/data/2.5/weather?q=Pisz&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";
    //Jezioro Konorowskie, Konorowo
    /*!
     * \brief Link do aktualnej pogody dla jeziora Koronowskiego
     */
    QString myURL2 = "http://api.openweathermap.org/data/2.5/weather?q=Koronowo&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";


    // Linki do miejsc [prognoza pogody]


    //Forecast:
    //Jezioro żywieckie, Żywiec
    /*!
     * \brief Link do prognozowanej pogody dla jeziora Żywieckiego
     */
    QString myURL00 = "https://api.openweathermap.org/data/2.5/onecall?lat=49.6853&lon=19.1924&exclude=daily,alerts,minutely&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";
    //Jezioro Roś, Pisz
    /*!
     * \brief Link do prognozowanej pogody dla jeziora Roś
     */
    QString myURL11 = "https://api.openweathermap.org/data/2.5/onecall?lat=53.6274&lon=21.8125&exclude=daily,alerts,minutely&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";
    //Jezioro Konorowskie, Konorowo
    /*!
     * \brief Link do prognozowanej pogody dla jeziora Koronowskiego
     */
    QString myURL22 = "https://api.openweathermap.org/data/2.5/onecall?lat=53.3137&lon=17.937&exclude=daily,alerts,minutely&units=metric&mode=json&lang=pl&appid=36496bad1955bf3365448965a42b9eac";

    /*!
     * \brief Interfejs użytkownika
     */
    Ui::MainWindow *ui;

    /*!
     * \brief Scena do animacji strzałek
     */
    QGraphicsScene *scene;
    /*!
     * \brief Sluzy do animacji strzalek
     */
    QTimer *timer;

    //QPropertyAnimation *animation;
    //QPropertyAnimation *animation_button;

    //QLabel *label_map;

private slots:
//public:

    /*!
     * \brief  Odebranie odpowiedzi z aktualna pogodą
     */
    void reload_data(QNetworkReply *reply);
    /*!
     * \brief  Odebranie odpwiedzi z prognoza pogody
     */
    void reload_data_forecast(QNetworkReply *reply);
    /*!
     * \brief  Wysłanie zapytania dla jeziora Żywieckiego - aktualna pogoda
     */
    bool check_for_an_update();
    /*!
     * \brief  Wysłanie zapytania dla jeziora Żywieckiego - prognoza pogody
     */
    bool check_for_an_update_forecast();
    /*!
     * \brief  Wysłanie zapytania dla jeziora Roś - aktualna pogoda
     */
    bool check_for_an_update1();
    /*!
     * \brief  Wysłanie zapytania dla jeziora Roś - prognoza pogody
     */
    bool check_for_an_update_forecast1();
    /*!
     * \brief  Wysłanie zapytania dla jeziora Koronowskiego - aktualna pogoda
     */
    bool check_for_an_update2();
    /*!
     * \brief  Wysłanie zapytania dla jeziora Koronowskiego - prognoza pogody
     */
    bool check_for_an_update_forecast2();
    /*!
     * \brief  Strona glowna aplikacji, mapa Polski z jeziorami
     */
    void page1();
    /*!
     * \brief  Strona szczegółowa jeziora Żywieckiego
     */
    void page2();
    /*!
     * \brief  Strona szczegółowa jeziora Roś
     */
    void page3();
    /*!
     * \brief  Strona szczegółowa jeziora Koronowskiego
     */
    void page4();
    /*!
     * \brief  Strona ładowania informacji, ekran oczekiwania
     */
    void page_loading();
    /*!
     * \brief  Funkcja obsługująca zawartosc danej strony
     */
    void page_content(int page_number);
};


#endif // MAINWINDOW_H
