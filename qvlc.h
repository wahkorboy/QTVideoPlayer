#ifndef QVLC_H
#define QVLC_H
#include<vlc/vlc.h>

#include <QObject>

class QTimer;

class QVlc : public QObject
{
    Q_OBJECT

protected:
    ~QVlc();
    QVlc(QObject *parent=nullptr);
    virtual void setPosition(float position)=0;
    virtual void setState(libvlc_state_t state)=0;
    virtual void setDuration(libvlc_time_t duration)=0;
    virtual void setCurrentTime(libvlc_time_t time)=0;

    libvlc_instance_t *m_instance;
    libvlc_media_player_t *m_media_player;
    libvlc_media_t *m_media;
    libvlc_state_t m_media_state;
    libvlc_time_t m_duration;
    QStringList m_playlist;
    QString m_media_location;
    QWidget *m_video_widget;
    QTimer *m_timer;

    float m_position;
    int m_index;


private:
    QTimer *makeTimer();


};

#endif // QVLC_H
