#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialcommunication.h"

#define TIMER_TIME 1000

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setConnectButton(bool state){connectButtonStatus = state;}
    void setDisconnectButton(bool state){disconnectButtonStatus = state;}

    bool getConnectButton(){return connectButtonStatus;}
    bool getDisconnectButton(){return disconnectButtonStatus;}

public slots:
    void comboBoxSetup();       // Port selection update
    void connectButton();       // Connect button action
    void disconnectButton();    // Disconnect button action

private:
    Ui::MainWindow *ui;
    SerialCommunication *serial;
    QTimer *timer;          // timer for port refresh

    bool connectButtonStatus;
    bool disconnectButtonStatus;
};


#endif // MAINWINDOW_H


