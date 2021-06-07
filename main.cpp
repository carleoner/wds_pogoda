#include "mainwindow.h"
#include <QApplication>

/*!
 * \file
 * \brief plik główny main
 */

/*!
 * \brief Utworzenie okna, rozpoczęcie pracy aplikacji
 */

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Pogoda"));
    w.show();
    return a.exec();
}
