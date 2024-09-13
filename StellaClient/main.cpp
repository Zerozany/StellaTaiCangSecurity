#include "widget.h"

#include <QApplication>

auto main(int argc, char *argv[]) ->int
{
    QApplication a(argc, argv);
    Widget w{};
    w.show();
    return QApplication::exec();
}
