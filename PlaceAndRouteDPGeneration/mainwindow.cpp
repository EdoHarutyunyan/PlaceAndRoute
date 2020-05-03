#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Symmetric.h"
#include "InterDigitation.h"
#include "MinimumArea.h"
#include "Temperature.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Design patters");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_symmetric_released()
{
    m_symmetric = new Symmetric;
    this->hide();
    m_symmetric->show();
}

void MainWindow::on_interDegitation_released()
{
	//m_interDigitation = new InterDigitation;
	//this->hide();
	//m_interDigitation->show();
}

void MainWindow::on_minimumArea_released()
{
    emit MinimumAreaChosen();
}

void MainWindow::on_temperature_released()
{
    emit TemperatureChosen();
}

