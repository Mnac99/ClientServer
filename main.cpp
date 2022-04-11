#include "serverrr.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName(Serverrr::tr(" Server"));
    Serverrr s;
    s.show();
    return a.exec();
}
