#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QCoreApplication>
#include <QSettings>
#include <QDir>

class FileManager
{
public:
    FileManager();
    int getInitialVolume();
    void setInitialVolume(int volume);

    void setMediaPath(QString path);
    QString getMediaPath();

    void savePlaylist(QStringList names,QString path=nullptr);
    QStringList getPlaylist(QString path=nullptr);
    QString getDefaultPlaylist();
    void setPlaylistPath(QString path);
    QString getPlaylistPath();
protected:
    QString appDir;
    QString initialPathFile;

private:
};

#endif // FILEMANAGER_H
