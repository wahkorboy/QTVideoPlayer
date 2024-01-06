#include "qvlccore.h"
#include <QWidget>
#include <QUrl>
#include <QDebug>



QVlcCore::QVlcCore(QObject *parent):QVlc {parent}
{
}

void QVlcCore::setIndex(int index)
{
    if(m_index !=index){
        m_index=index;
        emit indexChanged(m_index);
    }

}
bool QVlcCore::setMedia(const QString &media){
    m_media_location=media;
    libvlc_media_release(m_media);
    libvlc_media_parse_flag_t flag;
    if(m_media_location.startsWith("http",Qt::CaseInsensitive) ||
        m_media_location.startsWith("rtp",Qt::CaseInsensitive)){
        flag = libvlc_media_parse_network;
        m_media=libvlc_media_new_location(m_instance,QUrl(m_media_location).url().toStdString().c_str());

    }
    else{
        flag =libvlc_media_parse_local;
        m_media=libvlc_media_new_location(m_instance,QUrl::fromLocalFile(m_media_location).url().toStdString().c_str());
    }
    libvlc_media_player_set_media(m_media_player,m_media);
    return libvlc_media_parse_with_options(m_media,flag,-1)==0;
}

void QVlcCore::setPosition(float position){
    if(m_position !=position){
        m_position=position;
        emit positionChanged(int(m_position * 100.0f));
    }
}
void QVlcCore::setState(libvlc_state_t state){
    if(m_media_state != state){
        m_media_state=state;
        emit stateChanged(m_media_state);
    }
}
void QVlcCore::setDuration(libvlc_time_t duration){
    if(m_duration != duration){
        m_duration=duration;
        emit durationChanged(m_duration);
    }
}

void QVlcCore::setCurrentTime(libvlc_time_t time)
{
    emit currentTimeChanged(time);
}
int QVlcCore::getNextIndex(int index) const{
    if(index ==m_playlist.size()-1) return 0;
    return index+1;
}
int QVlcCore::getPrevIndex(int index) const{
    return index<=0?m_playlist.size()-1:index-1;
}


