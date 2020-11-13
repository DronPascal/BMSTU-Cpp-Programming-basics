#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWindow>
#include <QQmlContext>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include "fileSystemModel.h"
#include "playlistHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/images/window_icon.png"));
    qmlRegisterUncreatableType<DisplayFileSystemModel>("io.qt.examples.quick.controls.filesystembrowser", 1, 0,
                                                           "FileSystemModel", "Cannot create a FileSystemModel instance.");
    qmlRegisterType<playlistHandler>("my.mo.playlisthandler", 1, 0, "PlaylistHandler");
    QQmlApplicationEngine engine;

    QFileSystemModel* displayModel = new DisplayFileSystemModel;
    displayModel->setRootPath("/");
    displayModel->setRootPath("c:/Users/MiAir/");
    displayModel->setResolveSymlinks(true);

    engine.rootContext()->setContextProperty("fileSystemModel", displayModel);
    engine.rootContext()->setContextProperty("rootPathIndex", displayModel->index(displayModel->rootPath()));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}


/*
    const QStringList & musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    const QUrl musicUrl = QUrl::fromLocalFile(musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first());
    rootContext->setContextProperty(QStringLiteral("musicUrl"), musicUrl);

    const QStringList arguments = QCoreApplication::arguments();
    const QUrl commandLineUrl = arguments.size() > 1 ? QUrl::fromLocalFile(arguments.at(1)) : QUrl();
    rootContext->setContextProperty(QStringLiteral("url"), commandLineUrl);
*/
