#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "serialcommunication.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // window setup
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized); // maximizes the window (works sometimes)

    // buttons setup
    setConnectButton(true);
    setDisconnectButton(false);
    setTextEditor(false);

    // serial communication setup
    this->serial = new SerialCommunication;
    timer = new QTimer(this);
    timer->start(TIMER_TIME);
    connect(timer, SIGNAL(timeout()), this, SLOT(comboBoxSetup()));
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::connectButton);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::disconnectButton);

    // terminal setup
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::transferData);
    connect(serial, SIGNAL(readDataAvailable(QString)) , this, SLOT(sendReadDataToDisplay(QString)));
    connect(serial, SIGNAL(infoDataAvailable(QString)) , this, SLOT(sendInfoDataToDisplay(QString)));
    connect(serial, SIGNAL(connectionTestStatus(bool)) , this, SLOT(setConnectionStatus(bool)));
    ui->lineEdit->setMaxLength(30);

    // info table setup
    setSerialInfoTable("None", "None", "None", "None");

    // robot settings
    this->robotVariables = new RobotVariables;

    // interpreter
    this->interpreter = new Interpreter(robotVariables);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// function for port update
void MainWindow::comboBoxSetup(){
    if (serial->getPorts() == true){                         // get available ports (return true if connected port disappeared)
        serial->closeSerialPort();
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
        serial->openSerialPort();
    }
}

// function for disconnect button action
void MainWindow::disconnectButton(){
    if (getDisconnectButton() == true){
        serial->closeSerialPort();
    }
}

void MainWindow::setConnectionStatus(bool status){
    if (status == true){
        setConnectButton(false);
        setDisconnectButton(true);
        setTextEditor(true);
        sendInfoDataToDisplay("Connected");
        setSerialInfoTable(serial->currentPortName(), "9600", "8", "2");
    } else {
        setConnectButton(true);
        setDisconnectButton(false);
        setTextEditor(false);
        sendInfoDataToDisplay("Disconnected");
        setSerialInfoTable("None", "None", "None", "None");
    }
}

void MainWindow::transferData(){
    QString data = ui->lineEdit->displayText();
    string validatedData;
    validatedData.clear();
    ui->lineEdit->clear();
    errorFunction( interpreter->interpretation(data.toStdString(), &validatedData) );

    if (validatedData.size() > 0){
        sendWriteDataToDisplay( QString::fromStdString( validatedData ) );
    }
}

void MainWindow::sendWriteDataToDisplay(QString data){
    ui->plainTextEdit->appendPlainText("<<  ");
    ui->plainTextEdit->insertPlainText(data);
    serial->writeData(data);
}

void MainWindow::sendInfoDataToDisplay(QString data){
    ui->plainTextEdit->appendPlainText("INFO:  ");
    ui->plainTextEdit->insertPlainText(data);
}

void MainWindow::sendReadDataToDisplay(QString data){
    ui->plainTextEdit->appendPlainText(">>  ");
    ui->plainTextEdit->insertPlainText(data);
}


void MainWindow::setConnectButton(bool state){
    connectButtonStatus = state;
    if(connectButtonStatus == false)
        ui->pushButton->setStyleSheet("background-color: black");
    else
        ui->pushButton->setStyleSheet("background-color: grey");
}
void MainWindow::setDisconnectButton(bool state){
    disconnectButtonStatus = state;
    if(disconnectButtonStatus == false)
        ui->pushButton_2->setStyleSheet("background-color: black");
    else
        ui->pushButton_2->setStyleSheet("background-color: grey");
}
void MainWindow::setTextEditor(bool state){
    textEditorStatus = state;
    if(textEditorStatus == false){
        ui->lineEdit->setReadOnly(true);
        ui->lineEdit->setPlaceholderText("Disconnected");

    }else{
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit->setPlaceholderText("Enter command");
    }
}

void MainWindow::setSerialInfoTable(QString port, QString baudRate, QString dataBits, QString stopBits){
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();

    ui->lineEdit_6->insert(port);
    ui->lineEdit_7->insert(baudRate);
    ui->lineEdit_8->insert(dataBits);
    ui->lineEdit_9->insert(stopBits);
}

void MainWindow::errorFunction(int error){
    switch (error){
    case 0 :
        break;
    case 1:
        sendInfoDataToDisplay("Wrong format of function");
        break;
    case 2:
        sendInfoDataToDisplay("Function does not exist");
        break;
    case 3:
        sendInfoDataToDisplay("Argument outside limits");

    case 4:
        sendInfoDataToDisplay("Position unreachable");
        break;
    default:
        break;
    }
}
