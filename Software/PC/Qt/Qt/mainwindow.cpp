#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "serialcommunication.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(comboBoxSetup()));
    timer->start(TIMER_TIME);


    this->setWindowState(Qt::WindowMaximized); //Maximizes the window
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::comboBoxSetup(){
    serial.getPorts();
    if (ui->comboBox->count() != serial.portCounter()){
        ui->comboBox->clear();
        for (int i = 0; i < serial.portCounter(); i++){
            ui->comboBox->addItem(serial.portName(i));
        }
    }
}


