#include "logic.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Logic logic;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("logic", &logic);
    engine.load(QUrl(QStringLiteral("qrc:/qml/screens_loader.qml")));

    return app.exec();
}
