#include "UserGeneratedPattern.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGridLayout>
#include <QGraphicsSimpleTextItem>

namespace
{
	static const QString greenPath = ":/PlaceAndRouteDPGeneration/Resources/green.png";
	static const QString bluePath = ":/PlaceAndRouteDPGeneration/Resources/blue.jpg";
	static const QString yellowPath = ":/PlaceAndRouteDPGeneration/Resources/yellow.png";
	static const QString redPath = ":/PlaceAndRouteDPGeneration/Resources/red.jpg";
}

UserGeneratedPattern::UserGeneratedPattern(QWidget* parent)
	: QWidget(parent)
	, m_graphicsView(new QGraphicsView(this))
	, m_scene(new QGraphicsScene(this))
	, m_backButton(new QPushButton("< Back", this))
	, m_detailsButton(new QPushButton("Details", this))
	, m_browseButton(new QPushButton("Browse", this))
	, m_placeButton(new QPushButton("Place", this))
	, m_routeButton(new QPushButton("Route", this))
	, m_browseLineEdit(new QLineEdit(this))
	, m_rowFactorLineEdit(new QLineEdit(this))
	, m_columnFactorLineEdit(new QLineEdit(this))
	, m_rowFactorLabel(new QLabel("Row factor", this))
	, m_columnFactorLabel(new QLabel("Column factor", this))
	, m_topLeftComboBox(new QComboBox(this))
	, m_topRightComboBox(new QComboBox(this))
	, m_bottomLeftComboBox(new QComboBox(this))
	, m_bottomRightComboBox(new QComboBox(this))
	, m_topLeftPic(greenPath)
	, m_topRightPic(bluePath)
	, m_bottomLeftPic(yellowPath)
	, m_bottomRightPic(redPath)
{
	Initialize();

	connect(m_backButton, SIGNAL(released()), this, SLOT(on_Back_released()));
	connect(m_browseButton, SIGNAL(released()), this, SLOT(on_Browse_released()));
	connect(m_detailsButton, SIGNAL(released()), this, SLOT(\on_Details_released()));
	connect(m_placeButton, SIGNAL(released()), this, SLOT(on_Place_released()));
	connect(m_routeButton, SIGNAL(released()), this, SLOT(on_Route_released()));

	connect(m_topLeftComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text) { m_topLeftPic.load(GetPatternPicPath(text)); });
	connect(m_topRightComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text) { m_topRightPic.load(GetPatternPicPath(text)); });
	connect(m_bottomLeftComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text) { m_bottomLeftPic.load(GetPatternPicPath(text)); });
	connect(m_bottomRightComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text) { m_bottomRightPic.load(GetPatternPicPath(text)); });
}

void UserGeneratedPattern::Place(const uint32_t row, const uint32_t column, QGraphicsRectItem* area)
{

}

void UserGeneratedPattern::Route(const std::vector<std::vector<uint32_t>>& idsAdj)
{
}

std::vector<std::vector<uint32_t>> UserGeneratedPattern::Parse(QString&& text)
{
	return std::vector<std::vector<uint32_t>>();
}

std::vector<std::pair<uint32_t, uint32_t>> UserGeneratedPattern::AreaGeneration()
{
	return std::vector<std::pair<uint32_t, uint32_t>>();
}

void UserGeneratedPattern::Initialize()
{
	QGridLayout* mainLayout = new QGridLayout();

	QFrame* line = new QFrame();
	line->setGeometry(QRect(/* ... */));
	line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
	line->setFrameShadow(QFrame::Sunken);

	QLabel* title = new QLabel("Generate pattern", this);
	title->setStyleSheet("font: bold");
	title->setStyleSheet("font-size: 20px");

	// row0
	int row = 0;
	mainLayout->addWidget(m_backButton, row, 0, 1, 1);
	mainLayout->addWidget(title, row, 1, 1, 4, Qt::AlignCenter);
	++row;
	// row1
	mainLayout->addWidget(line, row, 0, 1, 5);
	++row;
	// row2
	mainLayout->addWidget(m_detailsButton, row, 0, 1, 1);
	++row;
	// row3
	mainLayout->addWidget(new QLabel("Top left", this), row, 1, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Top right", this), row, 2, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Bottom left", this), row, 3, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Bottom right", this), row, 4, 1, 1, Qt::AlignCenter);

	++row;
	// row4
	mainLayout->addWidget(m_topLeftComboBox, row, 1, 1, 1);
	mainLayout->addWidget(m_topRightComboBox, row, 2, 1, 1);
	mainLayout->addWidget(m_bottomLeftComboBox, row, 3, 1, 1);
	mainLayout->addWidget(m_bottomRightComboBox, row, 4, 1, 1);

	QGridLayout* patternLayout = new QGridLayout();
	QLabel* topLeftLabel = new QLabel(this);
	QLabel* topRightLabel = new QLabel(this);
	QLabel* bottomLeftLabel = new QLabel(this);
	QLabel* bottomRightLabel = new QLabel(this);

	topLeftLabel->setPixmap(m_topLeftPic);
	topRightLabel->setPixmap(m_topRightPic);
	bottomLeftLabel->setPixmap(m_bottomLeftPic);
	bottomRightLabel->setPixmap(m_bottomRightPic);

	patternLayout->addWidget(topLeftLabel, 0, 0, 1, 1);
	patternLayout->addWidget(topRightLabel, 0, 1, 1, 1);
	patternLayout->addWidget(bottomLeftLabel, 1, 0, 1, 1);
	patternLayout->addWidget(bottomRightLabel, 1, 1, 1, 1);

	// row4,5
	mainLayout->addLayout(patternLayout, row, 6, 2, 2);

	row += 2;
	// row6
	mainLayout->addWidget(m_rowFactorLabel, row, 1, 1, 1);
	mainLayout->addWidget(m_rowFactorLineEdit, row, 2, 1, 1, Qt::AlignLeft);

	++row;
	// row7
	mainLayout->addWidget(m_rowFactorLabel, row, 1, 1, 1);
	mainLayout->addWidget(m_rowFactorLineEdit, row, 2, 1, 1, Qt::AlignLeft);
	mainLayout->addWidget(m_browseButton, row, 3, 1, 1);
	mainLayout->addWidget(m_browseLineEdit, row, 4, 1, 1);

	++row;
	// row8-15
	mainLayout->addWidget(m_graphicsView, row, 0, 8, 8);
	mainLayout->addWidget(m_placeButton, 14, 9, 1, 1);
	mainLayout->addWidget(m_routeButton, 15, 9, 1, 1);

	setLayout(mainLayout);

	showMaximized();
	setWindowTitle("Generate pattern");

	m_topLeftComboBox->addItem(QIcon(greenPath), "green");
	m_topLeftComboBox->addItem(QIcon(yellowPath), "yellow");
	m_topLeftComboBox->addItem(QIcon(bluePath), "blue");
	m_topLeftComboBox->addItem(QIcon(redPath), "red");

	m_topRightComboBox->addItem(QIcon(bluePath), "blue");
	m_topRightComboBox->addItem(QIcon(greenPath), "green");
	m_topRightComboBox->addItem(QIcon(yellowPath), "yellow");
	m_topRightComboBox->addItem(QIcon(redPath), "red");

	m_bottomLeftComboBox->addItem(QIcon(yellowPath), "yellow");
	m_bottomLeftComboBox->addItem(QIcon(greenPath), "green");
	m_bottomLeftComboBox->addItem(QIcon(bluePath), "blue");
	m_bottomLeftComboBox->addItem(QIcon(redPath), "red");

	m_bottomRightComboBox->addItem(QIcon(redPath), "red");
	m_bottomRightComboBox->addItem(QIcon(yellowPath), "yellow");
	m_bottomRightComboBox->addItem(QIcon(bluePath), "blue");
	m_bottomRightComboBox->addItem(QIcon(greenPath), "green");

	SetStyleSheets();
}


void UserGeneratedPattern::SetStyleSheets()
{
	QPixmap pixmap(":/PlaceAndRouteDPGeneration/Resources/basket.png");
	QIcon ButtonIcon(pixmap);
	m_detailsButton->setIcon(ButtonIcon);
	m_detailsButton->setIconSize(QSize(65, 65));

	QPixmap bkgnd(":/PlaceAndRouteDPGeneration/Resources/background.jpg");
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_placeButton->setStyleSheet("background-color: beige");
	m_routeButton->setStyleSheet("background-color: beige");
	m_browseButton->setStyleSheet("background-color: beige");
	m_backButton->setStyleSheet("background-color: #4682B4");
}

Qt::GlobalColor UserGeneratedPattern::GetGlobalColorByType(const Cell::Type type) const
{
	Qt::GlobalColor color{};

	switch (type)
	{
	case Cell::Type::Green:
		color = Qt::GlobalColor::green;
		break;
	case Cell::Type::Yellow:
		color = Qt::GlobalColor::yellow;
		break;
	case Cell::Type::Blue:
		color = Qt::GlobalColor::blue;
		break;
	case Cell::Type::Red:
		color = Qt::GlobalColor::red;
		break;
	default:
		assert(false);
		break;
	}

	return color;
}

void UserGeneratedPattern::on_Back_released()
{

}

void UserGeneratedPattern::on_Browse_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
	m_browseLineEdit->setText(fileName);
}

void UserGeneratedPattern::on_Details_released()
{
}

void UserGeneratedPattern::on_Place_released()
{
	if (m_rowFactorLineEdit->text().isEmpty() || m_columnFactorLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Warning!", "Row factor and/or Column factor are empty!");
		return;
	}

	QRect rect(QPoint(0, 0), QSize(50, 50));
	QGraphicsRectItem* area = new QGraphicsRectItem(rect);
	area->setPos(50, 50);

	Place(m_rowFactorLineEdit->text().toUInt(), m_columnFactorLineEdit->text().toUInt(), area);

	m_graphicsView->setScene(m_scene);
}

void UserGeneratedPattern::on_Route_released()
{
}

QString UserGeneratedPattern::GetPatternPicPath(const QString& boxText)
{
	if (boxText == "green")
	{
		return greenPath;
	}
	if (boxText == "yellow")
	{
		return yellowPath;
	}
	if (boxText == "red")
	{
		return redPath;
	}
	if (boxText == "blue")
	{
		return bluePath;
	}
}


void UserGeneratedPattern::onUserGeneratedPatternChosen()
{
	show();
}
