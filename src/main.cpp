#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>

#include "jsonmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto jsonModel = new JsonModel(&engine);
    
    // fallback to build directory
    QString jsonPath = ":/resources/demo_data.json";
    if (!QFile::exists(jsonPath)) {
        jsonPath = "resources/demo_data.json";
    }
    
    jsonModel->loadJson(jsonPath);
    engine.rootContext()->setContextProperty("jsonModel", jsonModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("JsonTreeViewChallenge", "Main");

    return app.exec();
}
