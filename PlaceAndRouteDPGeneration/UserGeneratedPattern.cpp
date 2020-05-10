#include "UserGeneratedPattern.h"

#include <string>
#include <sstream>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGridLayout>
#include <QApplication>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>

namespace
{
	static const QString greenPath = ":/PlaceAndRouteDPGeneration/Resources/green.png";
	static const QString bluePath = ":/PlaceAndRouteDPGeneration/Resources/blue.jpg";
	static const QString yellowPath = ":/PlaceAndRouteDPGeneration/Resources/yellow.png";
	static const QString redPath = ":/PlaceAndRouteDPGeneration/Resources/red.png";

	static const uint32_t cellHeight = 50U;
	static const uint32_t cellWidth = 50U;
}

UserGeneratedPattern::UserGeneratedPattern(QWidget* parent)
	: QWidget(parent)
	, m_parser(std::make_shared<Parser>())
	, m_router(std::make_shared<Router>())
	, m_graphicsView(new QGraphicsView(this))
	, m_scene(new QGraphicsScene(this))
	, m_backButton(new QPushButton("< Back", this))
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
	, m_topLeftLabel(new QLabel(this))
	, m_topRightLabel(new QLabel(this))
	, m_bottomLeftLabel(new QLabel(this))
	, m_bottomRightLabel(new QLabel(this))
	, m_topLeftPic(greenPath)
	, m_topRightPic(bluePath)
	, m_bottomLeftPic(yellowPath)
	, m_bottomRightPic(redPath)
{
	Initialize();

	connect(m_backButton, SIGNAL(released()), this, SLOT(on_Back_released()));
	connect(m_browseButton, SIGNAL(released()), this, SLOT(on_Browse_released()));
	connect(m_placeButton, SIGNAL(released()), this, SLOT(on_Place_released()));
	connect(m_routeButton, SIGNAL(released()), this, SLOT(on_Route_released()));

	connect(m_topLeftComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text) 
		{ 
			if (m_topLeftPic.load(GetPatternPicPath(text))) { m_topLeftLabel->setPixmap(m_topLeftPic); }
		});
	connect(m_topRightComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text)
		{
			if (m_topRightPic.load(GetPatternPicPath(text))) { m_topRightLabel->setPixmap(m_topRightPic);}
		});
	connect(m_bottomLeftComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text)
		{
			if (m_bottomLeftPic.load(GetPatternPicPath(text))) { m_bottomLeftLabel->setPixmap(m_bottomLeftPic);}
		});
	connect(m_bottomRightComboBox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
		[=](const QString& text)
		{
			if (m_bottomRightPic.load(GetPatternPicPath(text))) { m_bottomRightLabel->setPixmap(m_bottomRightPic); }
		});
}

void UserGeneratedPattern::Place(const uint32_t row, const uint32_t column, QGraphicsRectItem* area)
{
	static uint32_t idNumber = 0u;

	for (size_t i = 0; i < 2 * row; ++i)
	{
		for (size_t j = 0; j < 2 * column; ++j)
		{
			QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, cellWidth, cellHeight, area);

			rect->setPen(QPen(Qt::black));
			rect->setBrush(QBrush(GetColorByIndex(i, j)));

			QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

			QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
			idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));
			
			rect->setPos(QPoint((j * cellWidth), (i * cellHeight)) + QPoint(25, 25));
		}
	}

	m_scene->addItem(area);
}

std::vector<std::pair<uint32_t, uint32_t>> UserGeneratedPattern::AreaGeneration()
{
	// Currently unused
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
	mainLayout->addWidget(m_backButton, row, 0);
	mainLayout->addWidget(title, row, 1, 1, 7, Qt::AlignCenter);
	++row;
	// row1
	mainLayout->addWidget(line, row, 0, 1, 7);

	++row;
	// row2
	mainLayout->addWidget(new QLabel("Top left", this), row, 1, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Top right", this), row, 2, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Bottom left", this), row, 3, Qt::AlignCenter);
	mainLayout->addWidget(new QLabel("Bottom right", this), row, 4, Qt::AlignCenter);

	++row;
	// row3
	mainLayout->addWidget(m_topLeftComboBox, row, 1);
	mainLayout->addWidget(m_topRightComboBox, row, 2);
	mainLayout->addWidget(m_bottomLeftComboBox, row, 3);
	mainLayout->addWidget(m_bottomRightComboBox, row, 4);

	QGridLayout* patternLayout = new QGridLayout();

	m_topLeftLabel->setFixedSize(300, 100);
	m_topRightLabel->setFixedSize(300, 100);
	m_bottomLeftLabel->setFixedSize(300, 100);
	m_bottomRightLabel->setFixedSize(300, 100);

	m_topLeftLabel->setPixmap(m_topLeftPic);
	m_topRightLabel->setPixmap(m_topRightPic);
	m_bottomLeftLabel->setPixmap(m_bottomLeftPic);
	m_bottomRightLabel->setPixmap(m_bottomRightPic);

	patternLayout->addWidget(m_topLeftLabel, 0, 0);
	patternLayout->addWidget(m_topRightLabel, 0, 1);
	patternLayout->addWidget(m_bottomLeftLabel, 1, 0);
	patternLayout->addWidget(m_bottomRightLabel, 1, 1);

	// row3, 4
	mainLayout->addLayout(patternLayout, row, 5, 2, 1);

	row += 2;
	// row5
	mainLayout->addWidget(m_rowFactorLabel, row, 1);
	mainLayout->addWidget(m_rowFactorLineEdit, row, 2, Qt::AlignLeft);

	++row;
	// row6
	mainLayout->addWidget(m_columnFactorLabel, row, 1);
	mainLayout->addWidget(m_columnFactorLineEdit, row, 2, Qt::AlignLeft);
	mainLayout->addWidget(m_browseButton, row, 7);
	mainLayout->addWidget(m_browseLineEdit, row, 6, Qt::AlignRight);

	++row;
	// row6-13
	mainLayout->addWidget(m_graphicsView, row, 0, 8, 7);
	mainLayout->addWidget(m_placeButton, 13, 7);
	mainLayout->addWidget(m_routeButton, 14, 7);

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
	QPixmap bkgnd(":/PlaceAndRouteDPGeneration/Resources/background.jpg");
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_placeButton->setStyleSheet("background-color: beige");
	m_routeButton->setStyleSheet("background-color: beige");
	m_browseButton->setStyleSheet("background-color: beige");
	m_backButton->setStyleSheet("background-color: #4682B4");
}

void UserGeneratedPattern::on_Back_released()
{
	close();
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
	m_scene->clear();
	m_graphicsView->setScene(m_scene);

	if (m_rowFactorLineEdit->text().isEmpty() || m_columnFactorLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Warning!", "Row factor and/or Column factor are empty!");
		return;
	}

	const uint32_t row = m_rowFactorLineEdit->text().toUInt();
	const uint32_t column = m_columnFactorLineEdit->text().toUInt();

	const uint32_t rectHeight = 2 * row * cellHeight + 50;
	const uint32_t rectWidht = 2 * column * cellWidth + 50;

	QRect rect(QPoint(0, 0), QSize(rectWidht, rectHeight));
	QGraphicsRectItem* area = new QGraphicsRectItem(rect);
	area->setPos(50, 50);

	Place(row, column, area);

	m_graphicsView->setScene(m_scene);
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

	return{};
}

Qt::GlobalColor UserGeneratedPattern::GetColorByIndex(const uint32_t i, const uint32_t j)
{
	const auto topLeftText = m_topLeftComboBox->currentText();
	const auto topRightText = m_topRightComboBox->currentText();
	const auto bottomLeftText = m_bottomLeftComboBox->currentText();
	const auto bottomRightText = m_bottomRightComboBox->currentText();

	Qt::GlobalColor color{};

	if (i % 2 == 0)
	{
		if (j % 2 == 0)
		{
			color = GetColorByText(m_topLeftComboBox->currentText());
		}
		else
		{
			color = GetColorByText(m_bottomLeftComboBox->currentText());
		}
	}
	else 
	{
		if (j % 2 == 0)
		{
			color = GetColorByText(m_topRightComboBox->currentText());
		}
		else
		{
			color = GetColorByText(m_bottomRightComboBox->currentText());
		}
	}

	return color;
}

Qt::GlobalColor UserGeneratedPattern::GetColorByText(const QString& boxText)
{
	if (boxText == "green")
	{
		return Qt::GlobalColor::green;
	}
	if (boxText == "yellow")
	{
		return Qt::GlobalColor::yellow;
	}
	if (boxText == "red")
	{
		return Qt::GlobalColor::red;
	}
	if (boxText == "blue")
	{
		return Qt::GlobalColor::blue;
	}

	return{};
}

void UserGeneratedPattern::on_Route_released()
{
	QString fileName = m_browseLineEdit->text();
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "title", "file not opened");
		return;
	}

	QTextStream in(&file);
	QString text = in.readAll();

	std::vector<std::vector<uint32_t>> idsAdj = m_parser->Parse(std::move(text));

	QList<QGraphicsItem*> items = m_scene->items();
	QList<QGraphicsSimpleTextItem*> itemIds;
	std::vector<uint32_t> predecessor;
	std::vector<uint32_t> distance;

	QVector<QGraphicsLineItem*> lines = m_router->Route(idsAdj, items, cellWidth, cellHeight);

	for (const auto& line : lines)
	{
		m_scene->addItem(line);
	}

	m_graphicsView->setScene(m_scene);
}

void UserGeneratedPattern::onUserGeneratedPatternChosen()
{
	show();
}
