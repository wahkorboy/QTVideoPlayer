#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QFileInfo>
#include<QListWidgetItem>


#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QVlcPlayer;
class FileManager;
class RepeatMode;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void autoplay(int argc,char *argv[]);

protected:
    void closeEvent(QCloseEvent *event);
    void handleVideoDoubleClick(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *event) override;
private slots:

    void playlistItemDoubleClicked(QListWidgetItem *item);

    void on_actionClose_triggered();

    void on_actionAdd_triggered();

    void on_playlistWidget_clicked(const QModelIndex &index);

    void on_playPauseButton_clicked();

    void on_previousButton_clicked();

    void on_nextButton_clicked();

    void on_positionSlider_sliderMoved(int position);


    void on_volumeSlider_sliderMoved(int position);



    void on_addMediaButton_clicked();

    void on_savePlaylistButton_clicked();

    void on_clearPlaylistButton_clicked();

    void on_openPlaylistButton_clicked();

    void on_repeatButton_clicked();

    void on_shuffleButton_clicked();

    void on_volumeSlider_valueChanged(int value);



    void on_actionplaylist_triggered();

    void on_seekPrevButton_clicked();

    void on_seekNextButton_clicked();

private:
    void deletePressed();
    Ui::MainWindow *ui;
    void toggleFullscreen(QWidget *);
    bool isWidgetFullscreen=false;
    QRect originalGeomitry;
    QVlcPlayer *mMediaPlayer;
    FileManager *initial;
    RepeatMode *repeatmode;
    QString getTimeFormat(int64_t intDuration);
    int64_t currentDuration=0;
    QWidget *originalParent;
    void savePlaylist(QString path);
    void getPlaylist(QString path);
    void handleMusicEnd();
    int currentPosition=-1;
    void setPlaylist(QString path);
};
#endif // MAINWINDOW_H
