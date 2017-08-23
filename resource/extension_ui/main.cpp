#include "extension.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Extension w;
    w.show();

    return a.exec();
}
