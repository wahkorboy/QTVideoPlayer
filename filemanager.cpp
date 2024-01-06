#include "filemanager.h"
#include <QDebug>


FileManager::FileManager()
{
        // Get the current directory
        appDir = QDir::currentPath();

        // Create the file path for initial.ini
        initialPathFile = appDir + QDir::separator() + "initial.ini";
        if(!QFile::exists(initialPathFile)){
            // Create a QSettings object for reading/writing settings
            QSettings settings(initialPathFile,QSettings::IniFormat);
            settings.sync();
            settings.setValue("volume",25);
            settings.setValue("mediaPath",appDir);
            settings.setValue("playlistPath",appDir);
        }



}

int FileManager::getInitialVolume(){

        // Create a QSettings object for reading/writing settings
        QSettings settings(initialPathFile,QSettings::IniFormat);
        return settings.value("volume",75).toInt();
}

void FileManager::setInitialVolume(int volume){
        volume =volume %100;
        QSettings settings(initialPathFile,QSettings::IniFormat);
        settings.sync();
        settings.setValue("volume",volume);

}

void FileManager::setMediaPath(QString path)
{

        QSettings settings(initialPathFile,QSettings::IniFormat);
        settings.setValue("mediaPath",path);
}

QString FileManager::getMediaPath(){

        QSettings settings(initialPathFile,QSettings::IniFormat);
        return settings.value("mediaPath",appDir).toString();
}

void FileManager::savePlaylist(QStringList names, QString path)
{
        if(path==nullptr){
            path=appDir+QDir::separator()+"currentPlaylist.irl";
        }
        QFile playlist(path);
        if (!playlist.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Handle error
            qDebug()<<"error open file";
            return;
        }
        QTextStream out(&playlist);
        foreach (QString filePath, names) {
            out<<filePath<<"\n";
        }
        playlist.close();

}

QStringList FileManager::getPlaylist(QString path)
{
        if(path==nullptr){
            path=getDefaultPlaylist();
        }
        QStringList playlist; // Initialize an empty list to store file paths


        // Open the playlist file
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // Handle error
            qDebug() << "Error opening playlist file: " << path;
            return playlist; // Return empty list if opening fails
        }

        // Use QTextStream to read the file line by line
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine(); // Read a line from the file
            playlist.append(line); // Add the line to the playlist list
        }

        file.close(); // Close the file
        if(path != getDefaultPlaylist()){

            QFileInfo fileInfo(path);
            QString folder=fileInfo.absolutePath();
            setPlaylistPath(folder);
        }

        return playlist; // Return the list of file paths from the playlist

}

QString FileManager::getDefaultPlaylist()
{
        return appDir+QDir::separator()+"currentPlaylist.irl";
}

void FileManager::setPlaylistPath(QString path)
{
        QSettings settings(initialPathFile,QSettings::IniFormat);
        settings.setValue("playlistPath",path);
        qDebug()<<"playlistPath= "<<path;
}

QString FileManager::getPlaylistPath()
{
        QSettings settings(initialPathFile,QSettings::IniFormat);
        return settings.value("playlistPath",appDir).toString();

}


