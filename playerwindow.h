#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();

private:
    Ui::PlayerWindow *ui;
};

#endif // PLAYERWINDOW_H
