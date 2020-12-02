#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "sfmlview.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    //QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    qmlRegisterType<SFMLView>("SFML", 1, 0, "View");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
