﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <iostream>
#include <QDir>
#include <QDirIterator>
#include <filesystem>

#include "serialcommunication.h"
#include "interpreter.h"
#include "robotvariables.hpp"
#include "function.h"
#include "protocol.h"
#include "scriptlist.h"

#define TIMER_TIME 1000
#define PATH_SCRIPTS "/home/jchlopak/Robot-ISCARA/Software/Qt/scripts/"

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

    // ui widgets setup
    void setConnectButton(bool state);
    void setDisconnectButton(bool state);
    void setDeleteButton(bool state);
    void setUploadButton(bool state);
    void setNormalButtons();

    void setTextEditor(bool state);
    void setSerialInfoTable(QString port, QString baudRate, QString dataBits, QString stopBits);

    // ui widget status
    bool getConnectButton(){return connectButtonStatus;}
    bool getDisconnectButton(){return disconnectButtonStatus;}
    bool getTextEditor(){return textEditorStatus;}

private slots:
    void comboBoxSetup();       // port selection update
    void connectButton();       // connect button action
    void disconnectButton();    // disconnect button action
    void transferData();        // read data from text line
    void sendReadDataToDisplay(QString data);       // send serial read data text to terminal
    void sendInfoDataToDisplay(QString data);       // send info text to terminal
    void sendWriteDataToDisplay(QString data);      // send serial write data text to terminal
    void setConnectionStatus(bool status);          // ui setup depending on serial port status

    void on_pushButton_3_clicked();     // help button action
    void on_pushButton_4_clicked();     // clear button action

    void newScriptButton();
    void deleteButton();
    void uploadButton();
    void stopButton();
    void refreshButton();



    void on_listWidget_itemSelectionChanged();

private:
    Ui::MainWindow *ui              = nullptr;
    SerialCommunication *serial     = nullptr;
    QTimer *timer                   = nullptr;      // Timer for port refresh
    Interpreter *interpreter        = nullptr;
    RobotVariables *robotVariables  = nullptr;

    // ui widgets variables
    bool connectButtonStatus;
    bool disconnectButtonStatus;
    bool textEditorStatus;

    // support functions
    void errorFunction  (int error);
    void helpFunction   ();

    //
    void updateScriptList();


};

#endif // MAINWINDOW_H
