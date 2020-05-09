#include "InterDigitation.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsSimpleTextItem>
#include <QGridLayout>

InterDigitation::InterDigitation(QWidget* parent)
	: QWidget(parent)
	, m_parser(std::make_shared<Parser>())
	, m_router(std::make_shared<Router>())
	, m_graphicsView(new QGraphicsView(this))
	, m_scene(new QGraphicsScene(this))
	, m_backButton(new QPushButton("< Back", this))
	, m_detailsButton(new QPushButton("Details", this))
	, m_addButton(new QPushButton("Add", this))
	, m_browseButton(new QPushButton("Browse", this))
	, m_placeButton(new QPushButton("Place", this))
	, m_routeButton(new QPushButton("Route", this))
	, m_typeLabel(new QLabel("Type", this))
	, m_countLabel(new QLabel("Count", this))
	, m_widthLabel(new QLabel("Width", this))
	, m_heightLabel(new QLabel("Height", this))
	, m_typeComboBox(new QComboBox(this))
	, m_commonCentroidRadioButton(new QRadioButton("CommonCentroid", this))
	, m_periodicRadioButton(new QRadioButton("Periodic", this))
	, m_countLineEdit(new QLineEdit(this))
	, m_widthLineEdit(new QLineEdit(this))
	, m_heightLineEdit(new QLineEdit(this))
	, m_browseLineEdit(new QLineEdit(this))
{
	Initialize();

	connect(m_addButton, SIGNAL(released()), this, SLOT(on_Add_released()));
	connect(m_backButton, SIGNAL(released()), this, SLOT(on_Back_released()));
	connect(m_browseButton, SIGNAL(released()), this, SLOT(on_Browse_released()));
	connect(m_detailsButton, SIGNAL(released()), this, SLOT(on_Details_released()));
	connect(m_placeButton, SIGNAL(released()), this, SLOT(on_Place_released()));
	connect(m_routeButton, SIGNAL(released()), this, SLOT(on_Route_released()));
}

void InterDigitation::Place(const uint32_t row, const uint32_t column, QGraphicsRectItem* area)
{
	static uint32_t idNumber = 0U;
	const auto cells = m_groupCells.GetCells();

	// because all cells has the same width and height
	const uint32_t width = cells.begin()->second.back().GetWidth();
	const uint32_t height = cells.begin()->second.back().GetHeight();

	uint32_t less{};
	uint32_t more{};

	if (row <= column)
	{
		less = row;
		more = column;
	}
	else
	{
		less = column;
		more = row;
	}

	if (GetMode() == Mode::CommonCentroid)
	{
		std::vector<std::pair<Cell::Type, uint32_t>> oddCells;
		std::vector<std::pair<Cell::Type, uint32_t>> evenCells;
		oddCells.reserve(m_groupCells.GetCount());
		evenCells.reserve(m_groupCells.GetCount());

		for (const auto& cell : cells)
		{
			uint32_t cellsCount = cell.second.size();
			if (cellsCount % 2 == 1)
			{
				oddCells.push_back(std::make_pair<Cell::Type, uint32_t>(cell.second.back().GetType(), 1));
			}
			if (cellsCount != 1)
			{
				evenCells.push_back(std::make_pair<Cell::Type, uint32_t>(cell.second.back().GetType(), cellsCount / 2));
			}
		}

		for (size_t i = 0; i < less; ++i)
		{
			for (size_t j = 0; j < more / 2; ++j)
			{
				std::pair<Cell::Type, uint32_t> cell = evenCells.back();

				QGraphicsRectItem* rect1 = new QGraphicsRectItem(0, 0, width, height, area);
				QGraphicsRectItem* rect2 = new QGraphicsRectItem(0, 0, width, height, area);

				rect1->setPen(QPen(Qt::black));
				rect1->setBrush(QBrush(GetGlobalColorByType(cell.first)));

				rect2->setPen(QPen(Qt::black));
				rect2->setBrush(QBrush(GetGlobalColorByType(cell.first)));

				if (row <= column)
				{
					rect1->setPos(QPoint((j * width), (i * height)) + QPoint(25, 25));
					rect2->setPos(QPoint((column - j - 1) * width, (row - i - 1) * height) + QPoint(25, 25));
				}
				else
				{
					rect1->setPos(QPoint((i * width), (j * height)) + QPoint(25, 25));
					rect2->setPos(QPoint((column - i - 1) * width, (row - j - 1) * height) + QPoint(25, 25));
				}

				QString id1 = QString::fromStdString("Id" + std::to_string(idNumber++));
				QString id2 = QString::fromStdString("Id" + std::to_string(idNumber++));

				QGraphicsSimpleTextItem* idItem1 = new QGraphicsSimpleTextItem(id1, rect1);
				QGraphicsSimpleTextItem* idItem2 = new QGraphicsSimpleTextItem(id2, rect2);

				idItem1->setPos(((rect1->rect().topLeft() + rect1->rect().topRight()) / 2, (rect1->rect().topLeft() + rect1->rect().bottomLeft()) / 2));
				idItem2->setPos(((rect2->rect().topLeft() + rect2->rect().topRight()) / 2, (rect2->rect().topLeft() + rect2->rect().bottomLeft()) / 2));

				if (--evenCells.back().second == 0)
				{
					evenCells.pop_back();
				}
			}
		}

		if (!oddCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = oddCells.back();
			QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);

			rect->setPen(QPen(Qt::black));
			rect->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
			rect->setPos(QPoint((column / 2 * width), (row / 2 * height)) + QPoint(25, 25));

			QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

			QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
			idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));
		}

		uint32_t i = 0;
		while (!evenCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = evenCells.back();
			QGraphicsRectItem* rect1 = new QGraphicsRectItem(0, 0, width, height, area);
			QGraphicsRectItem* rect2 = new QGraphicsRectItem(0, 0, width, height, area);

			rect1->setPen(QPen(Qt::black));
			rect1->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));

			rect2->setPen(QPen(Qt::black));
			rect2->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));

			QString id1 = QString::fromStdString("Id" + std::to_string(idNumber++));
			QString id2 = QString::fromStdString("Id" + std::to_string(idNumber++));

			QGraphicsSimpleTextItem* idItem1 = new QGraphicsSimpleTextItem(id1, rect1);
			QGraphicsSimpleTextItem* idItem2 = new QGraphicsSimpleTextItem(id2, rect2);

			idItem1->setPos(((rect1->rect().topLeft() + rect1->rect().topRight()) / 2, (rect1->rect().topLeft() + rect1->rect().bottomLeft()) / 2));
			idItem2->setPos(((rect2->rect().topLeft() + rect2->rect().topRight()) / 2, (rect2->rect().topLeft() + rect2->rect().bottomLeft()) / 2));

			for (size_t j = 0; j < currentCell.second; ++j)
			{
				if (row <= column)
				{
					rect1->setPos(QPoint((column / 2 * width), ((i + j) * height)) + QPoint(25, 25));
					rect2->setPos(QPoint((column / 2 * width), (row - j - i - 1) * height) + QPoint(25, 25));
				}
				else
				{
					rect1->setPos(QPoint(((i + j) * width), (row / 2) * height) + QPoint(25, 25));
					rect2->setPos(QPoint(((column - i - j - 1) * width), (row / 2) * height) + QPoint(25, 25));
				}
			}

			++i;
			evenCells.pop_back();
		}
	}
	else
	{
		const uint32_t typesCount = cells.size();
		uint32_t i = 0;
		for (auto it = cells.begin(); it != cells.end(); ++it)
		{
			Cell currentCell = it->second.back();

			for (size_t j = 0; j < more; j += typesCount)
			{
				QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);

				rect->setPen(QPen(Qt::black));
				rect->setBrush(QBrush(GetGlobalColorByType(currentCell.GetType())));

				QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

				QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
				idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));

				if (row == 1)
				{
					// horizontal
					rect->setPos(QPoint(((i + j) * width), height) + QPoint(25, -25));
				}
				else
				{
					// vertical
					rect->setPos(QPoint(width, (i + j) * height) + QPoint(-25, 25));
				}
			}
			++i;
		}
	}

	m_scene->addItem(area);
}

std::vector<std::pair<uint32_t, uint32_t>> InterDigitation::AreaGeneration()
{
	std::vector<std::pair<uint32_t /*row*/, uint32_t /*column*/>> areaSize{};
	uint32_t allCellsCount{};
	uint32_t oddCellsCount{};

	auto groupCell = m_groupCells.GetCells();

	for (const auto& group : groupCell)
	{
		if (group.second.size() % 2 == 1)
		{
			++oddCellsCount;
		}

		allCellsCount += group.second.size();
	}

	if (GetMode() == Mode::CommonCentroid)
	{
		if (oddCellsCount > 1)
		{
			QMessageBox::warning(this, "AreaGeneration", "oddCellsCount > 1");
			return{};
		}

		if (oddCellsCount == 1)
		{
			uint32_t linesCount = 1;

			while (linesCount <= allCellsCount)
			{
				if (allCellsCount % linesCount != 0)
				{
					linesCount += 2;
					continue;
				}

				areaSize.push_back(std::make_pair(linesCount, allCellsCount / linesCount));

				linesCount += 2;
			}
		}
		else
		{
			uint32_t linesCount = 1;

			while (linesCount <= allCellsCount)
			{
				if (allCellsCount % linesCount != 0)
				{
					linesCount *= 2;
					continue;
				}

				areaSize.push_back(std::make_pair(linesCount, allCellsCount / linesCount));

				linesCount *= 2;
			}
		}
	}
	else
	{
		if (groupCell.size() == 1)
		{
			QMessageBox::warning(this, "AreaGeneration", "Given only one type");
			return{};
		}

		for (const auto& group : groupCell)
		{
			if (group.second.size() * groupCell.size() != m_groupCells.GetCount())
			{
				QMessageBox::warning(this, "AreaGeneration", "Given Types should has the same count of cells");
				return{};
			}
		}

		areaSize.push_back(std::make_pair(1, allCellsCount));
		areaSize.push_back(std::make_pair(allCellsCount, 1));
	}

	return areaSize;
}

void InterDigitation::Initialize()
{
	QGridLayout* mainLayout = new QGridLayout();

	QFrame* line = new QFrame();
	line->setGeometry(QRect(/* ... */));
	line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
	line->setFrameShadow(QFrame::Sunken);

	QLabel* title = new QLabel("INTER-DIGITATION", this);
	title->setStyleSheet("font: bold");
	title->setStyleSheet("font-size: 20px");

	// row0
	int row = 0;
	mainLayout->addWidget(m_backButton, row, 0, 1, 1);
	mainLayout->addWidget(title, row, 1, 1, 7, Qt::AlignCenter);
	++row;
	// row1
	mainLayout->addWidget(line, row, 0, 1, 7);
	++row;
	// row2
	mainLayout->addWidget(m_detailsButton, row, 7, 1, 1);
	++row;
	// row3
	mainLayout->addWidget(m_typeLabel, row, 3, Qt::AlignCenter);
	mainLayout->addWidget(m_countLabel, row, 4, Qt::AlignCenter);
	mainLayout->addWidget(m_widthLabel, row, 5, Qt::AlignCenter);
	mainLayout->addWidget(m_heightLabel, row, 6, Qt::AlignCenter);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 0);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 1);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 2);

	m_typeLabel->setMargin(0);
	m_countLabel->setMargin(0);
	m_widthLabel->setMargin(0);
	m_heightLabel->setMargin(0);

	m_typeLabel->setContentsMargins(0, 0, 0, 0);
	m_countLabel->setContentsMargins(0, 0, 0, 0);
	m_widthLabel->setContentsMargins(0, 0, 0, 0);
	m_heightLabel->setContentsMargins(0, 0, 0, 0);

	++row;
	// row4
	mainLayout->addWidget(m_typeComboBox, row, 3);
	mainLayout->addWidget(m_countLineEdit, row, 4);
	mainLayout->addWidget(m_widthLineEdit, row, 5);
	mainLayout->addWidget(m_heightLineEdit, row, 6);
	mainLayout->addWidget(m_addButton, row, 7, 1, 1);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 0);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 1);
	mainLayout->addItem(new QSpacerItem(150, 0), row, 2);
	++row;

	// row5, 6
	mainLayout->addWidget(m_commonCentroidRadioButton, row, 1);
	mainLayout->addWidget(m_browseButton, row, 7, 2, 1);
	mainLayout->addWidget(m_browseLineEdit, row, 6, 2, 1);
	mainLayout->addWidget(m_periodicRadioButton, ++row, 1);
	++row;
	// row7-14
	mainLayout->addWidget(m_graphicsView, row, 0, 8, 7);
	mainLayout->addWidget(m_placeButton, 13, 7);
	mainLayout->addWidget(m_routeButton, 14, 7);

	setLayout(mainLayout);

	showMaximized();
	setWindowTitle("Symmetric");

	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/green.png"), "green");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/yellow.png"), "yellow");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/blue.jpg"), "blue");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/red.png"), "red");

	// Set labels alignments
	m_countLineEdit->setAlignment(Qt::AlignRight);
	m_widthLineEdit->setAlignment(Qt::AlignRight);
	m_heightLineEdit->setAlignment(Qt::AlignRight);

	SetStyleSheets();
}

void InterDigitation::SetStyleSheets()
{
	QPixmap pixmap(":/PlaceAndRouteDPGeneration/Resources/basket.png");
	QIcon ButtonIcon(pixmap);
	m_detailsButton->setIcon(ButtonIcon);
	m_detailsButton->setIconSize(QSize(65, 65));

	QPixmap bkgnd(":/PlaceAndRouteDPGeneration/Resources/background.jpg");
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);

	m_addButton->setStyleSheet("background-color: beige");
	m_placeButton->setStyleSheet("background-color: beige");
	m_routeButton->setStyleSheet("background-color: beige");
	m_browseButton->setStyleSheet("background-color: beige");
	m_backButton->setStyleSheet("background-color: #4682B4");
}

void InterDigitation::AddGroupCells(
	const Cell::Type type,
	const uint32_t count,
	const uint32_t width,
	const uint32_t height)
{
	std::vector<Cell> createdCells;
	createdCells.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		uint32_t currentId = m_groupCells.GetCount() + 1;
		createdCells.push_back(Cell(type, width, height, currentId));
	}

	m_groupCells.AddCells(createdCells);
}

Qt::GlobalColor InterDigitation::GetGlobalColorByType(const Cell::Type type) const
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

void InterDigitation::SetMode(const Mode mode)
{
	m_mode = mode;
}

InterDigitation::Mode InterDigitation::GetMode() const
{
	return m_mode;
}

void InterDigitation::on_Add_released()
{
	// timer 
	Cell::Type type{};
	const auto typeIndex = m_typeComboBox->currentIndex();
	const auto count = m_countLineEdit->text().toUInt();
	const auto width = m_widthLineEdit->text().toUInt();
	const auto height = m_heightLineEdit->text().toUInt();

	switch (typeIndex)
	{
	case 0:
		type = Cell::Type::Green;
		break;
	case 1:
		type = Cell::Type::Yellow;
		break;
	case 2:
		type = Cell::Type::Blue;
		break;
	case 3:
		type = Cell::Type::Red;
		break;
	default:
		assert(false);
		break;
	}

	AddGroupCells(type, count, width, height);
}

void InterDigitation::on_Browse_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
	m_browseLineEdit->setText(fileName);
}

void InterDigitation::on_Back_released()
{
	close();
	//TODO
}

void InterDigitation::on_Details_released()
{
	// SHOW Details
}

void InterDigitation::on_Place_released()
{
	m_scene->clear();
	m_graphicsView->setScene(m_scene);

	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to place!");
		return;
	}

	if (m_commonCentroidRadioButton->isChecked())
	{
		SetMode(Mode::CommonCentroid);
	}
	else if (m_periodicRadioButton->isChecked())
	{
		SetMode(Mode::Periodic);
	}
	else
	{
		QMessageBox::warning(this, "Warning!", "Please choose placement mode");
		return;
	}

	std::vector<std::pair<uint32_t /*row*/, uint32_t /*column*/>> areaSizes = AreaGeneration();

	if (areaSizes.empty())
	{
		QMessageBox::warning(this, "Warning!", "Sorry, impossible to place these elements");
		return;
	}

	const auto cells = m_groupCells.GetCells();

	// because all cells has the same width and height
	const uint32_t width = cells.begin()->second.back().GetWidth();
	const uint32_t height = cells.begin()->second.back().GetHeight();

	uint32_t x = 50;
	uint32_t y = 50;
	for (const auto& areaSize : areaSizes)
	{
		const uint32_t rectHeight = areaSize.first * height + 50;
		const uint32_t rectWidht = areaSize.second * width + 50;

		QPoint topLeft(0, 0);
		QRect rect(topLeft, QSize(rectWidht, rectHeight));

		QGraphicsRectItem* area = new QGraphicsRectItem(rect);
		area->setPos(x, y);

		Place(areaSize.first, areaSize.second, area);

		x += (rectWidht + 100);
	}

	m_graphicsView->setScene(m_scene);
}

void InterDigitation::on_Route_released()
{
	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to route!");
		return;
	}

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

	auto cells = m_groupCells.GetCells();
	auto it = cells.begin();

	const uint32_t height = it->second.back().GetHeight();
	const uint32_t width = it->second.back().GetWidth();

	QVector<QGraphicsLineItem*> lines = m_router->Route(idsAdj, items, width, height);

	for (const auto& line : lines)
	{
		m_scene->addItem(line);
	}

	m_graphicsView->setScene(m_scene);
}

void InterDigitation::onInterDigitationChosen()
{
	show();
}
