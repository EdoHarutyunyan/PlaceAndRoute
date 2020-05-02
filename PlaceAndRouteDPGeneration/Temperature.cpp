#include "Temperature.h"
#include "ui_Temperature.h"

Temperature::Temperature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Temperature)
{
    ui->setupUi(this);
}

Temperature::~Temperature()
{
    delete ui;
}

void Temperature::onTemperatureChosen()
{

}
