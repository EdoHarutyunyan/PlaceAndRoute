#include "mainwindow.h"

#include "Symmetric.h"
#include "InterDigitation.h"
#include "UserGeneratedPattern.h"

#include <iostream>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, m_symmetricButton(new QPushButton(this))
	, m_interDigitationButton(new QPushButton(this))
	, m_userGeneratedPatternButton(new QPushButton(this))
{
	Initialize();

	connect(m_symmetricButton, SIGNAL(released()), this, SLOT(on_symmetric_released()));
	connect(m_interDigitationButton, SIGNAL(released()), this, SLOT(on_interDegitation_released()));
	connect(m_userGeneratedPatternButton, SIGNAL(released()), this, SLOT(on_userGeneratedPattern_released()));
}

void MainWindow::Initialize()
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
	QWidget* mainWidget = new QWidget();

	mainLayout->addItem(new QSpacerItem(100, 100));
	mainLayout->addWidget(m_symmetricButton);
	mainLayout->addWidget(m_interDigitationButton);
	mainLayout->addWidget(m_userGeneratedPatternButton);
	mainLayout->addItem(new QSpacerItem(100, 100));

	QPixmap pixmap(":/PlaceAndRouteDPGeneration/Resources/symmetric.png");
	QIcon ButtonIcon(pixmap);
	m_symmetricButton->setIcon(ButtonIcon);
	m_symmetricButton->setIconSize(pixmap.rect().size());

	QPixmap pixmap2(":/PlaceAndRouteDPGeneration/Resources/interdigitation.png");
	QIcon ButtonIcon2(pixmap2);
	m_interDigitationButton->setIcon(ButtonIcon2);
	m_interDigitationButton->setIconSize(pixmap2.rect().size());

	QPixmap pixmap3(":/PlaceAndRouteDPGeneration/Resources/UserGenerated.jpg");
	QIcon ButtonIcon3(pixmap3);
	m_userGeneratedPatternButton->setIcon(ButtonIcon3);
	m_userGeneratedPatternButton->setIconSize(pixmap3.rect().size());

	QPixmap bkgnd(":/PlaceAndRouteDPGeneration/Resources/background.jpg");
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);
	
	showMaximized();
	setWindowTitle("Design patters");
}

void MainWindow::on_symmetric_released()
{
    m_symmetric = new Symmetric;
    this->hide();
    m_symmetric->show();
}

void MainWindow::on_interDegitation_released()
{
	m_interDigitation = new InterDigitation;
	this->hide();
	m_interDigitation->show();
}

void MainWindow::on_userGeneratedPattern_released()
{
	m_userGeneratedPattern = new UserGeneratedPattern;
	this->hide();
	m_userGeneratedPattern->show();
}


