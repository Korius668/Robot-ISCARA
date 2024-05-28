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

public slots:
    void comboBoxSetup();

private:
    Ui::MainWindow *ui;
    SerialCommunication serial;
    QTimer *timer;
};


#endif // MAINWINDOW_H


