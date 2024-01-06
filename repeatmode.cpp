#include "repeatmode.h"

RepeatMode::RepeatMode()
{
    QSettings settings(initialPathFile,QSettings::IniFormat);
    repeatModes = new QStringList();
    repeatModes->append("Repeat All");
    repeatModes->append("Repeat Off");
    repeatModes->append("Repeat One");
    repeatIndex=getCurrentIndex();
    currentRepeatMode=repeatModes->at(repeatIndex);

    shuffleModes = new QStringList();
    shuffleModes->append("Shuffle Off");
    shuffleModes->append("Shuffle On");
    shuffleIndex=settings.value("shuffleMode",0).toInt();
}
QString RepeatMode::repeatButtonClicked(){
    repeatIndex =(repeatIndex+1)%3;
    currentRepeatMode=repeatModes->at(repeatIndex);
    setCurrentIndex();
    return currentRepeatMode;
}

QString RepeatMode::shuffleButtonClicked()
{
    // Create a QSettings object for reading/writing settings
    QSettings settings(initialPathFile,QSettings::IniFormat);
    shuffleIndex=settings.value("shuffleMode",0).toInt();
    shuffleIndex = (shuffleIndex+1)%2;
    settings.setValue("shuffleMode",shuffleIndex);
    return shuffleModes->at(shuffleIndex);

}
QString RepeatMode::getRepeatMode(){
    currentRepeatMode=repeatModes->at(repeatIndex);
    return currentRepeatMode;
}

QString RepeatMode::getShuffleMode()
{
    // Create a QSettings object for reading/writing settings
    QSettings settings(initialPathFile,QSettings::IniFormat);
    shuffleIndex=settings.value("shuffleMode",0).toInt();
    return shuffleModes->at(shuffleIndex);

}
void RepeatMode::setShuffleMode(int index)
{
    // Create a QSettings object for reading/writing settings
    QSettings settings(initialPathFile,QSettings::IniFormat);
    settings.setValue("shuffleMode",index);

}

int RepeatMode::getNextIndex(QStringList items, int currentIndex)
{
    if(shuffleIndex) return std::rand() % (items.size()-1);
    if(repeatIndex==2) return currentIndex;
    if(repeatIndex==1 && currentIndex==items.size()-1) return -1;
    return (currentIndex+1)% items.size();

}

int RepeatMode::getCurrentIndex()
{
    // Create a QSettings object for reading/writing settings
    QSettings settings(initialPathFile,QSettings::IniFormat);
    return settings.value("repeatMode",0).toInt();
}
void RepeatMode::setCurrentIndex()
{
    // Create a QSettings object for reading/writing settings
    QSettings settings(initialPathFile,QSettings::IniFormat);
    return settings.setValue("repeatMode",repeatIndex);
}
