#include "MinimumArea.h"
#include "ui_MinimumArea.h"

MinimumArea::MinimumArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinimumArea)
{
    ui->setupUi(this);
}

MinimumArea::~MinimumArea()
{
    delete ui;
}

void MinimumArea::onMinimumAreaChosen()
{

}
