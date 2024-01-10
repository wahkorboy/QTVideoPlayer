#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "qvlcplayer.h"
#include "filemanager.h"
#include "repeatmode.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Assuming ui->widgetVideo is an instance of MyWidget
    mMediaPlayer =new QVlcPlayer(this);
    repeatmode = new RepeatMode();
    initial=repeatmode;
    ui->widgetVideo->installEventFilter(this);
    ui->repeatButton->setText(repeatmode->getRepeatMode());
    ui->shuffleButton->setText(repeatmode->getShuffleMode());
    ui->volumeSlider->setValue(initial->getInitialVolume());
    mMediaPlayer->setVideoWidget(ui->widgetVideo);
    mMediaPlayer->setVolume(ui->volumeSlider->value());
    connect(mMediaPlayer,&QVlcPlayer::empty_playlist,this,[&]()
            {
        this->on_actionAdd_triggered();
    });
    connect(mMediaPlayer,&QVlcCore::indexChanged,this,[&](int index){
        ui->playlistWidget->setCurrentRow(index);
    });
    connect(mMediaPlayer,&QVlcCore::stateChanged,this,[&](libvlc_state_t state){
        if(state==libvlc_Ended){
            handleMusicEnd();
            return;
        }
        if(state==libvlc_Paused)
            ui->playPauseButton->setText("Play");
        else
            ui->playPauseButton->setText("Pause");
    });
    connect(mMediaPlayer,&QVlcCore::currentTimeChanged,this,[&](int64_t time){
        ui->currentPositionLabel->setText(getTimeFormat(time));
        ui->positionSlider->setValue(time);
        currentPosition=time;
    });
    connect(mMediaPlayer,&QVlcCore::positionChanged,this,[&](int position)
            {
               // ui->positionSlider->setValue(position);
            });
    connect(mMediaPlayer,&QVlcCore::durationChanged,this,[&](int64_t duration){
        currentDuration=duration;

        ui->durationPositionLabel->setText(getTimeFormat(duration) );
        ui->positionSlider->setMaximum(duration);
        });
    getPlaylist(initial->getDefaultPlaylist());
    connect(ui->playlistWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::playlistItemDoubleClicked);


}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug()<<"Evetn"<<event->type();
    if (obj == ui->widgetVideo && event->type() == QEvent::MouseButtonDblClick)
    {
        toggleFullscreen(ui->widgetVideo);
        return true;  // Event handled
    }else if(obj == ui->widgetVideo && event->type() == QEvent::KeyPress){

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key()==Qt::Key_Escape && this->isWidgetFullscreen){
            toggleFullscreen(ui->widgetVideo);
        }
    }


    // Pass the event to the base class
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
if(    event->key() == Qt::Key_Delete){

        deletePressed();
}

}

void MainWindow::playlistItemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"item double click";
    // Add your logic here, for example, play the selected item
    int index = ui->playlistWidget->row(item);
    mMediaPlayer->playAt(index);
}

void MainWindow::deletePressed()
{
    QListWidgetItem* currentItem=ui->playlistWidget->currentItem();
    if(!currentItem) return;
    int currentIndex=ui->playlistWidget->row(currentItem);
    mMediaPlayer->removeMediaAt(currentIndex);
    ui->playlistWidget->takeItem(currentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete repeatmode;
}

void MainWindow::autoplay(int argc,char *argv[])
{
    qDebug()<<"argc= "<<argc;
    for(int i=0;i<argc;i++)
        qDebug()<<argv[i];
    //autoPlayPath=path;
    on_clearPlaylistButton_clicked();
    if(argc<1) return;
    QString filePath=QString::fromLocal8Bit(argv[1]);
    if(filePath.endsWith("irl")){
        qDebug()<<"irl file"<<filePath;
        setPlaylist(filePath);
    }else{
        mMediaPlayer->addMedia(filePath);
        ui->playlistWidget->addItem(filePath);
        mMediaPlayer->playAt(0);
        mMediaPlayer->play();

    }
}



// Don't forget to release the resources when the MainWindow is closed
void MainWindow::closeEvent(QCloseEvent *event)
{

    QMainWindow::closeEvent(event);
}



void MainWindow::toggleFullscreen(QWidget *widget)
{
    this->isWidgetFullscreen = !this->isWidgetFullscreen;
    if (this->isWidgetFullscreen==false) {
        // If the widget is already in fullscreen, exit fullscreen mode
        //ui->widgetVideo->hide();
        widget->showNormal();
        ui->hrcontainer->addWidget(widget);
        //ui->widgetVideo->setParent(ui->hrcontainer);  // Set the parent to hrcontainer
    } else {
        // If the widget is not in fullscreen, enter fullscreen mode
        widget->setParent(nullptr);  // Remove the parent, making it a standalone window
        widget->showFullScreen();
    }
}


void MainWindow::on_actionClose_triggered()
{
    close();
}


void MainWindow::on_actionAdd_triggered()
{
    on_addMediaButton_clicked();

}


void MainWindow::on_playlistWidget_clicked(const QModelIndex &index)
{
    return;
    mMediaPlayer->playAt(index.row());
}


void MainWindow::on_playPauseButton_clicked()
{

    mMediaPlayer->playPauseToggle();
}


void MainWindow::on_previousButton_clicked()
{
    mMediaPlayer->prev();
}


void MainWindow::on_nextButton_clicked()
{
    mMediaPlayer->next();
}


void MainWindow::on_positionSlider_sliderMoved(int position)
{
    mMediaPlayer->setPosition(position);
}




void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    mMediaPlayer->setVolume(position);
    initial->setInitialVolume(position);
}


QString MainWindow::getTimeFormat(int64_t intDuration){
    intDuration /=1000; // cut milliseconds
    int hours = intDuration /3600;
    int minutes = (intDuration%3600)/60;
    int secconds = intDuration %60;
    QString duration;
    if(hours>0){
        duration = QString("%1:%2:%3")
                       .arg(hours,2,10,QChar('0'))
                       .arg(minutes,2,10,QChar('0'))
                       .arg(secconds,2,10,QChar('0'));
    }else{
        duration = QString("%1:%2")
                       .arg(minutes,2,10,QChar('0'))
                       .arg(secconds,2,10,QChar('0'));
    }
    return duration;
}

void MainWindow::savePlaylist(QString path)
{
    if(ui->playlistWidget->count() ==0){
        return;}
    QStringList files;
    for(int i=0;i<ui->playlistWidget->count();i++){
        QString path =ui->playlistWidget->item(i)->text();
        files.append(path);
    }
    initial->savePlaylist(files,path);
}
void MainWindow::getPlaylist(QString path)
{
    ui->playlistWidget->clear();
    mMediaPlayer->clearPlaylist();
    QStringList names=initial->getPlaylist(path);
    for(int i=0;i<names.size();i++){
        ui->playlistWidget->addItem(names.at(i));
        mMediaPlayer->addMedia(names.at(i));
    }
}





void MainWindow::on_addMediaButton_clicked()
{
    QString path=initial->getMediaPath();
    QStringList files = QFileDialog::getOpenFileNames(this, "vlc mp4", path);
    int nFile=files.size();
    for(int i=0;i<nFile;i++){
        mMediaPlayer->addMedia(files.at(i));
        ui->playlistWidget->addItem(files.at(i));
        if(i==0){
            QString folder;
            QFileInfo fileInfo(files.at(0));
            folder=fileInfo.absolutePath();
            initial->setMediaPath(folder);

        }

    }
    savePlaylist(initial->getDefaultPlaylist());
    return;
}


void MainWindow::on_savePlaylistButton_clicked()
{
    QString path;
    path=QFileDialog::getSaveFileName(this,"save playlist","","All Files (*)");
    if(path.isEmpty())
        return;
    if(!path.endsWith(".irl"))
        path+=".irl";


    QFileInfo file(path);
    QString playlistPath=file.absolutePath();
    initial->setPlaylistPath(playlistPath);

    savePlaylist(path);

}


void MainWindow::on_clearPlaylistButton_clicked()
{
    ui->playlistWidget->clear();
    mMediaPlayer->clearPlaylist();
    QStringList empty;
    initial->savePlaylist(empty);
}


void MainWindow::on_openPlaylistButton_clicked()
{
    QStringList filters;
    filters << "*.irl"; // Add other filters if needed
    QString path = QFileDialog::getOpenFileName(this, "Open IRL File", initial->getPlaylistPath(), filters.join(";"));
    setPlaylist(path);

}
void MainWindow::setPlaylist(QString path){
    if(!QFile::exists(path)) return;
    QFileInfo file(path);
    QString playlistPath=file.absoluteFilePath();
    initial->setPlaylistPath(playlistPath);
    getPlaylist(path);
    savePlaylist(initial->getDefaultPlaylist());

}
void MainWindow::handleMusicEnd(){
    int index=ui->playlistWidget->currentRow();
    QStringList files;
    for(int i=0;i<ui->playlistWidget->count();i++){
        QString path =ui->playlistWidget->item(i)->text();
        files.append(path);
    }
    int nextIndex=repeatmode->getNextIndex(files,index);
    if(nextIndex==-1) return;
    ui->playlistWidget->setCurrentRow(nextIndex);
    ui->playlistWidget->item(nextIndex)->setSelected(true);
    mMediaPlayer->playAt(nextIndex);
}

void MainWindow::on_repeatButton_clicked()
{
    ui->repeatButton->setText(repeatmode->repeatButtonClicked());
}


void MainWindow::on_shuffleButton_clicked()
{
    ui->shuffleButton->setText(repeatmode->shuffleButtonClicked());
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{
    mMediaPlayer->setVolume(value);
    initial->setInitialVolume(value);
}




void MainWindow::on_actionplaylist_triggered()
{
    if(ui->playlistframe->isHidden())
        ui->playlistframe->show();
    else
            ui->playlistframe->hide();
}


void MainWindow::on_seekPrevButton_clicked()
{
    mMediaPlayer->seekPrev();
}


void MainWindow::on_seekNextButton_clicked()
{
    mMediaPlayer->seekNext();
}

