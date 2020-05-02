#include "Symmetric.h"
#include "ui_Symmetric.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsSimpleTextItem>

#include <QLabel>
#include <QLine>
#include <QSpacerItem>

Symmetric::Symmetric(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Symmetric)
	, m_scene(new QGraphicsScene(this))
{
    //ui->setupUi(this);
	//			  
	//ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/green.png"), "green");
	//ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/yellow.png"), "yellow");
	//ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/blue.jpg"), "blue");
	//ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/red.jpg"), "red");

	QGridLayout* mainLayout = new QGridLayout();

	// row0

	int row = 0;
	mainLayout->addWidget(new QPushButton("back"), row, 0, 1, 1);
	mainLayout->addWidget(new QLabel("SYMMETRIC"), row, 1, 1, 4);
	++row;
	// row1
	mainLayout->addWidget(new QPushButton("----------------------------"), row, 0, 1, 5);
	++row;
	// row2
	mainLayout->addWidget(new QPushButton("Details"), row, 0, 1, 1);
	++row;
	// row3
	mainLayout->addWidget(new QPushButton("Type"), row, 0, 1, 1);
	mainLayout->addWidget(new QPushButton("Count"), row, 1, 1, 1);
	mainLayout->addWidget(new QPushButton("Width"), row, 2, 1, 1);
	mainLayout->addWidget(new QPushButton("Height"), row, 3, 1, 1);
	mainLayout->addItem(new QSpacerItem(25, 25), row, 4, 1, 1);
	++row;
	// row4
	mainLayout->addWidget(new QComboBox(), row, 0, 1, 1);
	mainLayout->addWidget(new QLineEdit(), row, 1, 1, 1);
	mainLayout->addWidget(new QLineEdit(), row, 2, 1, 1);
	mainLayout->addWidget(new QLineEdit(), row, 3, 1, 1);
	mainLayout->addWidget(new QPushButton("Add"), row, 4, 1, 1);
	++row;
	// row5, 6
	mainLayout->addWidget(new QRadioButton("Horizontal"), row, 0, 1, 1);
	mainLayout->addWidget(new QPushButton("Browse"), row, 1, 2, 1);
	mainLayout->addWidget(new QLineEdit(), row, 2, 2, 1);
	mainLayout->addWidget(new QRadioButton("Vertical"), ++row, 0, 1, 1);
	++row;
	// row7
	mainLayout->addWidget(new QGraphicsView(), row, 0, 2, 4);
	mainLayout->addWidget(new QPushButton("Place"), row, 4, 1, 1);
	mainLayout->addWidget(new QPushButton("Route"), ++row, 4, 1, 1);


	//mainLayout->setRowStretch(0, 0);
	//mainLayout->setRowStretch(1, 0);
	//mainLayout->setRowStretch(2, 0);
	//mainLayout->setRowStretch(3, 0);
	//mainLayout->setRowStretch(4, 0);
	//mainLayout->setRowStretch(5, 0);
	//mainLayout->setRowStretch(6, 0);
	//mainLayout->setRowStretch(7, 20);
	//mainLayout->setRowStretch(8, 20);


	setLayout(mainLayout);

	showMaximized();
	setWindowTitle("Symmetric");
}

Symmetric::~Symmetric()
{
    delete ui;
}

void Symmetric::Place(const uint32_t row, const uint32_t column, QGraphicsRectItem* area)
{ 
	std::vector<std::pair<Cell::Type, uint32_t>> oddCells;
	std::vector<std::pair<Cell::Type, uint32_t>> evenCells;
	oddCells.reserve(m_groupCells.GetCount());
	evenCells.reserve(m_groupCells.GetCount());

	const auto cells = m_groupCells.GetCells();

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

	// because all cells has the same width and height
	uint32_t width = cells.begin()->second.back().GetWidth();
	uint32_t height = cells.begin()->second.back().GetHeight();
	int32_t x{};
	
	static uint32_t idNumber = 0U;

	if (GetSymmetryLine() == SymmetryLine::Vertical)
	{
		for (size_t i = 0; i < row; ++i)
		{
			for (size_t j = 0; j < column / 2; ++j)
			{
				std::pair<Cell::Type, uint32_t> cell = evenCells.back();
				
				QGraphicsRectItem* rect1 = new QGraphicsRectItem(0, 0, width, height, area);
				QGraphicsRectItem* rect2 = new QGraphicsRectItem(0, 0, width, height, area);
				
				rect1->setPen(QPen(Qt::black));
				rect1->setBrush(QBrush(GetGlobalColorByType(cell.first)));
				rect1->setPos(QPoint((j * width), (i * height)) + QPoint(25, 25));

				rect2->setPen(QPen(Qt::black));
				rect2->setBrush(QBrush(GetGlobalColorByType(cell.first)));
				rect2->setPos(QPoint((column - j - 1) * width, (i * height)) + QPoint(25, 25));

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

		uint32_t i = 0;

		while (!oddCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = oddCells.back();
			QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);
			rect->setPen(QPen(Qt::black));
			rect->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
			rect->setPos(QPoint((column / 2 * width), (i * height)) + QPoint(25, 25));

			QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

			QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
			idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));
			
			oddCells.pop_back();
			++i;
		}

		while (!evenCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = evenCells.back();

			for (size_t j = 0; j < 2 * currentCell.second; ++j)
			{
				QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);
				rect->setPen(QPen(Qt::black));
				rect->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
				rect->setPos(QPoint((column / 2 * width), ((i + j) * height)) + QPoint(25, 25));

				QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

				QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
				idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));
			}

			evenCells.pop_back();
			++i;
		}
	}
	else
	{
		for (size_t i = 0; i < row / 2; ++i)
		{
			for (size_t j = 0; j < column; ++j)
			{
				std::pair<Cell::Type, uint32_t> currentCell = evenCells.back();
				QGraphicsRectItem* rect1 = new QGraphicsRectItem(0, 0, width, height, area);
				QGraphicsRectItem* rect2 = new QGraphicsRectItem(0, 0, width, height, area);

				rect1->setPen(QPen(Qt::black));
				rect1->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
				rect1->setPos(QPoint((j * width), (i * height)) + QPoint(25, 25));

				rect2->setPen(QPen(Qt::black));
				rect2->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
				rect2->setPos(QPoint((j * width), ((row - i - 1) * height)) + QPoint(25, 25));

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

		uint32_t i = 0;

		while (!oddCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = oddCells.back();
			QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);

			rect->setPen(QPen(Qt::black));
			rect->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
			rect->setPos(QPoint((i* width), (row / 2 * height)) + QPoint(25, 25));

			QString id = QString::fromStdString("Id" + std::to_string(idNumber++));

			QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
			idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));

			oddCells.pop_back();
			++i;
		}

		while (!evenCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = evenCells.back();

			for (size_t j = 0; j < 2 * currentCell.second; ++j)
			{
				QGraphicsRectItem* rect = new QGraphicsRectItem(0, 0, width, height, area);
				rect->setPen(QPen(Qt::black));
				rect->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));
				rect->setPos(QPoint((i + j)* width, (row* height)) + QPoint(25, 25));

				QString id = QString::fromStdString("Id" + std::to_string(idNumber));
				++idNumber;

				QGraphicsSimpleTextItem* idItem = new QGraphicsSimpleTextItem(id, rect);
				idItem->setPos(((rect->rect().topLeft() + rect->rect().topRight()) / 2, (rect->rect().topLeft() + rect->rect().bottomLeft()) / 2));
			}

			evenCells.pop_back();
			++i;
		}
	}

	m_scene->addItem(area);
}

std::vector<std::pair<QString, QString>> Symmetric::Parse(QString&& text)
{
	std::vector<std::pair<QString, QString>> ids;
	std::vector<std::string> tokens;
	std::stringstream stream(text.toStdString());
	std::string item;

	while (std::getline(stream, item, ' '))
	{
		if (!item.empty())
		{
			tokens.push_back(item);
		}
	}

	return ids;
}

std::vector<std::string> Tokenize(std::string&& line)
{
	std::vector<std::string> tokens;
	std::stringstream stream(line);
	std::string item;

	while (std::getline(stream, item, ' '))
	{
		if (!item.empty())
		{
			if (item.back() == ',' || item.back() == ';')
			{
				item.pop_back();

				if (item.back() == ')')
				{
					item.pop_back();
				}
			}
			if (item.front() == '(')
			{
				item = item.substr(1, item.size());
			}
			tokens.push_back(item);
		}
	}

	return tokens;
}


std::vector<std::pair<uint32_t, uint32_t>> Symmetric::AreaGeneration()
{
	std::vector<std::pair<uint32_t /*row*/, uint32_t /*column*/>> areaSize{};
	uint32_t allCellsCount{};
	uint32_t lineCellsCount{};

	auto groupCell = m_groupCells.GetCells();

	for (const auto& group : groupCell)
	{
		if (group.second.size() % 2 == 1)
		{
			++lineCellsCount;
		}

		allCellsCount += group.second.size();
	}

	if (lineCellsCount > 0)
	{
		while (lineCellsCount <= allCellsCount / 2)
		{
			if (allCellsCount % lineCellsCount != 0)
			{
				lineCellsCount += 2;
				continue;
			}

			if (GetSymmetryLine() == SymmetryLine::Vertical)
			{
				areaSize.push_back(std::make_pair(lineCellsCount, allCellsCount / lineCellsCount));
			}
			else
			{
				areaSize.push_back(std::make_pair(allCellsCount / lineCellsCount, lineCellsCount));
			}

			lineCellsCount += 2;
		}
	}
	else
	{
		uint32_t linesCount = 1;

		while (linesCount <= allCellsCount / 2)
		{
			if (allCellsCount % linesCount != 0)
			{
				linesCount *= 2;
				continue;
			}

			if (GetSymmetryLine() == SymmetryLine::Vertical)
			{
				areaSize.push_back(std::make_pair(linesCount, allCellsCount / linesCount));
			}
			else
			{
				areaSize.push_back(std::make_pair(allCellsCount / linesCount, linesCount));
			}

			linesCount *= 2;
		}
	}

	return areaSize;
}

void Symmetric::SetSymmetryLine(const SymmetryLine symmetryLine)
{
	m_symmetryLine = symmetryLine;
}

Symmetric::SymmetryLine Symmetric::GetSymmetryLine() const
{
	return m_symmetryLine;
}

void Symmetric::AddGroupCells(
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

Qt::GlobalColor Symmetric::GetGlobalColorByType(const Cell::Type type) const
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

void Symmetric::onSymmetricChosen()
{
	show();
}

void Symmetric::on_ADD_released()
{
	Cell::Type type{};
	const auto typeIndex = ui->TypeComboBox->currentIndex();
	const auto count = ui->CountLineEdit->text().toUInt();
	const auto width = ui->WidthLineEdit->text().toUInt();
	const auto height = ui->HeightLineEdit->text().toUInt();

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

void Symmetric::on_Place_released()
{
	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to place!");
		return;
	}

	if (ui->Vertical->isChecked())
	{
		SetSymmetryLine(SymmetryLine::Vertical);
	}
	else if (ui->Horizontal->isChecked())
	{
		SetSymmetryLine(SymmetryLine::Horizontal);
	}
	else
	{
		QMessageBox::warning(this, "Warning!", "Please choose symmetry line");
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
	//QGraphicsView* a = new ..;
	//a->setScene(m_scene);
	//a->show();
	ui->graphicsView->setScene(m_scene);
};

void Symmetric::on_browse_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
	ui->path->setText(fileName);
}

void Symmetric::on_Route_released()
{
	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to route!");
		return;
	}

	QString fileName = ui->path->text();
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "title", "file not opened");
	}

	QTextStream in(&file);
	QString text = in.readAll();

	std::vector<std::pair<QString, QString>> ids = Parse(std::move(text));

	Route(ids);
}

void Symmetric::Route(const std::vector<std::pair<QString, QString>>& ids)
{
	QList<QGraphicsItem*> items = m_scene->items();
	QList<QGraphicsSimpleTextItem*> itemIds;

	for (QGraphicsItem* item : items)
	{
		QGraphicsSimpleTextItem* id = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);

		if (id)
		{
			itemIds.push_back(id);
		}
	}
}
