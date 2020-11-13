#include "playlistHandler.h"

playlistHandler::playlistHandler(QObject *parent) : QObject(parent)
{

}

void playlistHandler::openPlaylist(const QUrl &url)
{
    QString a = url.toString();
    a.remove("file:///");
    QFile file(a);
    qDebug() << "ook";
    if ((file.exists())&&(file.open(QIODevice::ReadOnly))) {
        QString str="";
        qDebug() << "oook";
        while(!file.atEnd())
        {
            str=str+file.readLine();
            qDebug() << str;
            str.remove('\r');
        }
        str += "\n";
        qDebug() << str;
        file.close();
        emit urlListUploaded(str);
    }
}

//void playlistHandler::savePlaylist(QUrl &url)
void playlistHandler::savePlaylist(QString album_name, QString recources)
{
    QString path;
    if (album_name.split("/").size() == 1) {    //если название альбома
        path = "C:/Users/MiAir/Music/" + album_name + ".alb";
    }
    else {
        path = album_name + ".alb";
    }           //было txt

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        recources.remove(recources.size() - 1, 1);
        file.write(recources.toStdString().c_str());
        file.close();
    }
}

void playlistHandler::deletePlaylist(QString album_name)
{
    album_name.remove("file:///");
    QFile file(album_name);
    qDebug() << "cpp - " << album_name;
    if (file.exists()) {
        file.remove();
    }
}
























