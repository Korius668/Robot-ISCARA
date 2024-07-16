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
    setNormalButtons();
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

    // script selection setup
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::newScriptButton);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::deleteButton);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::uploadButton);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::stopButton);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::refreshButton);
    updateScriptList();

    // info table setup
    setSerialInfoTable("NONE", "None", "None", "None");

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
        setSerialInfoTable("NONE", "None", "None", "None");
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

    if (port == "NONE")
        ui->lineEdit_6->setStyleSheet("color: red");
    else
        ui->lineEdit_6->setStyleSheet("color: green");

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

// help button action
void MainWindow::on_pushButton_3_clicked()
{
    helpFunction();
}

// clear button action
void MainWindow::on_pushButton_4_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::helpFunction(){
    vector<Function> functions;
    string communicate;
    string name;
    int parametersNumber;
    communicate.append("FUNCTIONS AVAILABLE:\n");

    functions = interpreter->getFunctionList();
    if ( functions.size() > 0 ){
        for (unsigned int i = 0 ; i < functions.size(); i++){
            name.clear();
            name.append( "- " );
            name.append( functions[i].getName() );
            parametersNumber = functions[i].getParametersNumber();
            name.append("( ");
            name.append( to_string(parametersNumber) );
            name.append(" parameters )\n");
            communicate.append( name );
        }
    }
    sendInfoDataToDisplay( QString::fromStdString(communicate) );
}


///////////////////////////////////////////////////////////////
/// SCRIPT EDITOR////////
///

void MainWindow::updateScriptList()
{
    QDirIterator it(PATH_SCRIPTS, QStringList() << "*.dat", QDir::Files);
    QString name;
    ui->listWidget->clear();


    while (it.hasNext()){
        name = it.nextFileInfo().fileName();
        name.truncate( name.lastIndexOf('.') );
        ui->listWidget->addItem(name);
    }
    ui->listWidget->setCurrentRow(-1);
    setDeleteButton(false);
    setUploadButton(false);

    ui->listWidget->sortItems();
}



void MainWindow::newScriptButton()
{
    ScriptList scriptlist;
    scriptlist.setModal(true);
    scriptlist.exec();

}

void MainWindow::deleteButton()
{
    string name;
    string path = PATH_SCRIPTS;
    name = ui->listWidget->currentItem()->text().toStdString();
    name.append(".dat");

    path.append(name);
    std::filesystem::remove(path);

    updateScriptList();
}

void MainWindow::uploadButton()
{

}

void MainWindow::stopButton()
{

}

void MainWindow::refreshButton()
{
    updateScriptList();
}


void MainWindow::setDeleteButton(bool status){
    if(status == false){
        ui->pushButton_6->setStyleSheet("background-color: black");
        ui->pushButton_6->setDisabled(true);
    }else{
        ui->pushButton_6->setStyleSheet("background-color: grey");
        ui->pushButton_6->setDisabled(false);
    }
}

void MainWindow::setUploadButton(bool status){
    if(status == false){
        ui->pushButton_7->setStyleSheet("background-color: black");
        ui->pushButton_7->setDisabled(true);
    }else{
        ui->pushButton_7->setStyleSheet("background-color: grey");
        ui->pushButton_7->setDisabled(false);
    }
}

void MainWindow::setNormalButtons()
{
    ui->pushButton->setStyleSheet("background-color: grey");
    ui->pushButton_2->setStyleSheet("background-color: grey");
    ui->pushButton_3->setStyleSheet("background-color: grey");
    ui->pushButton_4->setStyleSheet("background-color: grey");
    ui->pushButton_5->setStyleSheet("background-color: grey");
    ui->pushButton_6->setStyleSheet("background-color: grey");
    ui->pushButton_7->setStyleSheet("background-color: grey");
    ui->pushButton_8->setStyleSheet("background-color: grey");
    ui->pushButton_9->setStyleSheet("background-color: grey");
}


void MainWindow::on_listWidget_itemSelectionChanged()
{
    if ( ui->listWidget->currentRow() != -1 )
    {
        setDeleteButton(true);
        setUploadButton(true);
    }
    else
    {
        setDeleteButton(false);
        setUploadButton(false);
    }

}

