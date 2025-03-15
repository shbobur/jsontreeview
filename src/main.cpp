#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "treemodel.h"
#include "treeitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto apple = new TreeItem("Apple");
    auto iphone = new TreeItem("iPhone");
    auto mac = new TreeItem("Macbook");

    auto test = new TreeItem(123);
    auto test_num = new TreeItem("null");
    auto test_longstr = new TreeItem("test 1234256789 qwer567uasdfgh");

    auto treeModel = new TreeModel(&engine);
    treeModel->addTopLevelItem(apple);
    treeModel->addTopLevelItem(test);

    treeModel->addItem(apple, mac);
    treeModel->addItem(apple, iphone);
    treeModel->addItem(test, test_num);
    treeModel->addItem(test, test_longstr);

    engine.rootContext()->setContextProperty("treeModel", treeModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.loadFromModule("JsonTreeViewChallenge", "Main");

    return app.exec();
}
