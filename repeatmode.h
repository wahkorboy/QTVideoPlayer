#ifndef REPEATMODE_H
#define REPEATMODE_H
#include <QString>
#include <QStringList>
#include "filemanager.h"

class RepeatMode:public FileManager
{
public:
    RepeatMode();
    QString repeatButtonClicked();
    QString shuffleButtonClicked();
    int getNextIndex(QStringList items,int currentIndex);
    QString getRepeatMode();
    QString getShuffleMode();
protected:
    void setShuffleMode(int index);
private:
    QStringList *repeatModes;
    QStringList *shuffleModes;
    QString currentRepeatMode;
    int repeatIndex;
    int getCurrentIndex();
    void setCurrentIndex();
    int shuffleIndex;
};
#endif // REPEATMODE_H
