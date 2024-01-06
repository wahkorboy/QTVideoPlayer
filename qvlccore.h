#ifndef QVLCCORE_H
#define QVLCCORE_H
#include "qvlc.h"
#include <QtConcurrent>
#include <QThread>


class QVlcCore:public QVlc
{
    Q_OBJECT
signals:
    void positionChanged(int position);
    void indexChanged(int index);
    void stateChanged(libvlc_state_t state);
    void durationChanged(libvlc_time_t duration);
    void currentTimeChanged(libvlc_time_t time);

protected:
    QVlcCore(QObject *parent=nullptr);
    void setIndex(int index);
    bool setMedia(const QString &media);
    void setPosition(float position) override;
    void setState(libvlc_state_t state) override;
    void setDuration(libvlc_time_t duration) override;
    void setCurrentTime(libvlc_time_t time) override;
    int getNextIndex(int index) const;
    int getPrevIndex(int index) const;
private:


};

#endif // QVLCCORE_H
