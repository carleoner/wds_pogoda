#include "wind_arrow.h"
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>

/*!
 * \file
 * \brief Plik klasy strzałki
 *
 * W pliku źródłowym znajdują się polecenia
 * do stworzenia kształtu strzałki, inicjalizacji danych
 * oraz do animacji obiektu.
 */

/*!
 * Konstruktor ustawia wartości początkowych parametrów
 * szybkości i kątu kierunku wiatru. Losuje on również położenie
 * animowanego obiektu na scenie.
 *
 * \param[in] speed - szybkość poruszania się strzałek wyrazona w [m/s]
 * \param[in] angle_deg - kat kierunku poruszania się strzałek, wyrażony w stopniach
 */

Wind_arrow::Wind_arrow(double speed, int angle_deg)
{

    //random start of position and speed
    wind_speed = speed;
    angle = angle_deg + 180;

    //setTransformOriginPoint(0,0);

    setRotation(QRandomGenerator::global()->bounded(360 * 16));
    //angle = 0; //can be random
    //setRotation(angle);

    //int StartX = scene->sceneRect().center().toPoint().x();
    //int StartX = 300; //-320 środek -20 lewa -620 prawa
    //int StartY = 300; //-300 środek -125 góra -480 dół
    int StartX = -620 + rand() % 600;
    int StartY = -480 + rand() % 355;

    setPos(mapToParent(StartX, StartY));
}

Wind_arrow::~Wind_arrow()
{

}

/*!
 * \return Rozmiary obrysu obiektu, ograniczenie pola rysownia.
 */
QRectF Wind_arrow::boundingRect() const
{
    /*qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
                  */
    return QRect(0,0,120,120);
}


/*!
 * \param[in] rysownik
 */

void Wind_arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //https://forum.qt.io/topic/109749/how-to-create-an-arrow-in-qt/6
    QPoint start = QPoint(10, 50);
    QPoint end = QPoint(10, 0);

    painter->setRenderHint(QPainter::Antialiasing, true);

    qreal arrowSize = 20; // size of head
    painter->setPen(Qt::black);
    painter->setBrush(Qt::black);
    painter->setOpacity(0.4);

    QLineF line(end, start);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                            cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                            cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter->drawLine(line);
    painter->drawPolygon(arrowHead);



    //zakomentowane rysowanie chmury
    /*
    ///rysowanie chmury za pomoca gradientu
    //painter->setOpacity(0.5);

    QRadialGradient radialGrad(QPointF(60, 50), 30);
    radialGrad.setColorAt(0, Qt::gray);
    radialGrad.setColorAt(0.5, Qt::transparent);
    radialGrad.setColorAt(1, Qt::transparent);
    QRect rect_radial(0,0,100,100);
    painter->fillRect(rect_radial, radialGrad);

    QRadialGradient radialGrad1(QPointF(50, 50), 20);
    radialGrad1.setColorAt(0, Qt::gray);
    radialGrad1.setColorAt(0.5, Qt::transparent);
    radialGrad1.setColorAt(1, Qt::transparent);
    QRect rect_radial1(0,0,100,100);
    painter->fillRect(rect_radial1, radialGrad1);

    QRadialGradient radialGrad2(QPointF(70, 55), 25);
    radialGrad2.setColorAt(0, Qt::gray);
    radialGrad2.setColorAt(0.5, Qt::transparent);
    radialGrad2.setColorAt(1, Qt::transparent);
    QRect rect_radial2(0,0,100,100);
    painter->fillRect(rect_radial2, radialGrad2);


    QRectF rec = boundingRect();
    QBrush Brush(Qt::gray);

    if(!scene()->collidingItems(this).isEmpty())
        Collision();

    painter->setOpacity(0.5);
    painter->fillRect(rec, Brush);
    painter->drawRect(rec);
    */

}

/*!
 * \param[in] phase - krok animacji strzałek
 */

void Wind_arrow::advance(int phase)
{
    //jesli nie ma kroku
    if(!phase)
        return;
    QPointF location = this->pos();
    //qDebug() << location;


    setRotation(angle);
    //qDebug() << angle;

    setPos(mapToParent(0,-(wind_speed)));

    //setting coordinates X Y for Wind_arrow respawn
    int x=0,y=0;
    if(angle < 540 && angle >= 450)
    {
        x = 300 + (540-angle)*300/90;
        y = (540-angle)*300/90;
    }
    if(angle < 450 && angle >= 360)
    {
        x = 300 + (450-angle)*300/90;
        y = (450-angle)*300/90;
    }
    if(angle < 360 && angle >= 270)
    {
        x = 300 - (360-angle)*300/90;
        y = (360-angle)*300/90;
    }
    if(angle < 270 && angle >= 180)
    {
        x = (270-angle)*300/90;
        y = 300 + (270-angle)*300/90;
    }

    //przesuniecie chmury w tyl
    QLineF LineToStart(QPointF(x-300, y-300), location);
    if(LineToStart.length() > 400)
    {
        setPos(mapToParent(0, 600));
    }

}

///*!
// * \brief Funkcja kolizji
// *
// */

//void Wind_arrow::Collision()
//{
//    //this->wind_speed = 0;
//    //delete this;
//}
