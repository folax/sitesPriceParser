#include "sitespriceparser.h"
#include <QApplication>
#include <QScopedPointer>
#include <QTextCodec>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QScopedPointer<sitesPriceParserGUI> wnd(new sitesPriceParserGUI);
    wnd->show();

    return app.exec();
}

