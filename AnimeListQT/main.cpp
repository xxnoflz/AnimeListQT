#include "AnimeListQT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnimeListQT w;
    w.show();
    return a.exec();
}
