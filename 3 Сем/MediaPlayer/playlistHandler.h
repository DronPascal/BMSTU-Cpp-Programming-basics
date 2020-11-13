#ifndef PLAYLISTHANDLER_H
#define PLAYLISTHANDLER_H

#include <QObject>
#include <QFile>
#include <QUrl>
#include <QDebug>

class playlistHandler : public QObject
{
    Q_OBJECT
public:
    explicit playlistHandler(QObject *parent = nullptr);

    Q_INVOKABLE void openPlaylist(const QUrl &url);
    Q_INVOKABLE void savePlaylist(QString, QString);
    Q_INVOKABLE void deletePlaylist(QString);

Q_SIGNALS:
    void urlListUploaded(QString text);
};

#endif // PLAYLISTHANDLER_H
