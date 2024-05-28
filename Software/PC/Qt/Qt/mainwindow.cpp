#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "serialcommunication.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized); //Maximizes the window

    SerialCommunication ser;

    ser.getPorts();

    ui->comboBox->addItem("Hi");
}

MainWindow::~MainWindow()
{
    delete ui;
}






