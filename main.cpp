#include "XDAxis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XDAxis w;
    w.show();
    return a.exec();
}
