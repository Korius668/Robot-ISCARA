#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "serialcommunication.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // window setup
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized); //Maximizes the window

    this->connectButtonStatus = true;
    this->disconnectButtonStatus = false;

    // serial communication setup
    this->serial = new SerialCommunication;
    timer = new QTimer(this);
    timer->start(TIMER_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(comboBoxSetup()));
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::connectButton);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::disconnectButton);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// function for port update
void MainWindow::comboBoxSetup(){
    if (serial->getPorts() == true){                         // get available ports (return true if connected port disappeared)
        serial->closeSerialPort();
        setConnectButton(true);
        setDisconnectButton(false);
    }

    if (ui->comboBox->count() != serial->portCounter()){    // put available port names in box selection
        ui->comboBox->clear();
        for (int i = 0; i < serial->portCounter(); i++){
            ui->comboBox->addItem(serial->portName(i));
        }
    }
}

// function for connect button action
void MainWindow::connectButton(){
    if (getConnectButton() == true){
        serial->setPort( ui->comboBox->currentText() );
        if(serial->openSerialPort()){
            setConnectButton(false);
            setDisconnectButton(true);
        }
        serial->writeData("Test\n");
    }
}

// function for disconnect button action
void MainWindow::disconnectButton(){
    if (getDisconnectButton() == true){
        if(serial->closeSerialPort()){
            setConnectButton(true);
            setDisconnectButton(false);
        }
    }
}
