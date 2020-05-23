#include "velawindow.h"
#include <QApplication>
#include <QThread>

#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Création de l'interface et affichage
    VelaWindow w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    w.showFullScreen();

    return a.exec();
}
