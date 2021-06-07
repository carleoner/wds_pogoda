#ifndef WIND_ARROW_H
#define WIND_ARROW_H

/*!
 * \file
 * \brief Definicja klasy Wind_arrow
 *
 * Plik zawiera definicję klasy Wind_arrow,
 * która odpowiada za tworzenie obiektu
 * oraz animowanie go poprzez funkcję advance().
 *
 */

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

/*!
 * \brief Klasa modeluje pojęcie obiektu - strzałkę
 *
 * Pierwotnie elementem animowanym miała być chmura. W trakcie
 * realizacji projektu, pod wplywem spostrzeżeń
 * prowadzącego, koncepcja zmieniła się. Obiektem
 * animowanym jest ostatecznie strzałka z efektem półprzezroczystości.
 *
 * W klasie znajdują się informacje o kącie i szybkości
 * elementu.
 */

class Wind_arrow : public QGraphicsItem
{
public:

    /*!
     * \brief Inicjalizuje kąt kierunku i szybkość animowanego obiektu.
     */
    Wind_arrow(double speed, int angle_deg);

    /*!
     * \brief Metoda wywoływana przed usunięciem obiektu.
     */
    ~Wind_arrow();

    /*!
     * \brief Funkcja ogranicza pole rysowania.
     */
    QRectF boundingRect() const override;

    /*!
     * \brief Metoda rysująca kształt strzałki.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    /*!
     * \brief Metoda aktualizuje położenie animowanego obiektu.
     */
    virtual void advance(int phase) override;

private:

    /*!
     * \brief Zadany kąt kierunku przesuwania się obiektu, wyrażany w stopniach
     */
    qreal angle;
    /*!
     * \brief Zadana szybkość poruszania się obiektu, wyrażana w [m/s]
     */
    qreal wind_speed;
};

#endif // WIND_ARROW_H
