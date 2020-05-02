#include "InterDigitation.h"
#include "ui_InterDigitation.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsSimpleTextItem>

InterDigitation::InterDigitation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterDigitation),
	m_scene(new QGraphicsScene(this))
{
    ui->setupUi(this);

	ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/green.png"), "green");
	ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/yellow.png"), "yellow");
	ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/blue.jpg"), "blue");
	ui->TypeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/red.jpg"), "red");
}

InterDigitation::~InterDigitation()
{
    delete ui;
}

void InterDigitation::Place(const uint32_t row, const uint32_t column, QGraphicsRectItem* area)
{
	static uint32_t idNumber = 0U;
	const auto cells = m_groupCells.GetCells();
	
	// because all cells has the same width and height
	uint32_t width = cells.begin()->second.back().GetWidth();
	uint32_t height = cells.begin()->second.back().GetHeight();

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

		for (size_t i = 0; i < less; ++i)
		{
			for (size_t j = 0; j < more / 2; ++j)
			{
				std::pair<Cell::Type, uint32_t> cell = evenCells.back();

				Qt::GlobalColor color = GetGlobalColorByType(cell.first);
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
					rect2->setPos(QPoint((row - i - 1) * width, (column - j - 1) * height) + QPoint(25, 25));
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

		while (!evenCells.empty())
		{
			std::pair<Cell::Type, uint32_t> currentCell = evenCells.back();
			Qt::GlobalColor color = GetGlobalColorByType(currentCell.first);
			QGraphicsRectItem* rect1 = new QGraphicsRectItem(0, 0, width, height, area);
			QGraphicsRectItem* rect2 = new QGraphicsRectItem(0, 0, width, height, area);

			rect1->setPen(QPen(Qt::black));
			rect1->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));

			rect2->setPen(QPen(Qt::black));
			rect2->setBrush(QBrush(GetGlobalColorByType(currentCell.first)));

			for (size_t j = 0; j < currentCell.second; ++j)
			{
				if (row <= column)
				{
				/*	rect1->setPos(QPoint((column / 2 * width), (j* height)) + QPoint(25, 25));
					rect2->setPos(QPoint((column / 2 * width), ((row - j - 1) * height) + QPoint(25, 25));*/
					//m_scene->addRect(QRect((column / 2 * width), (j * height), width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
					//m_scene->addRect(QRect((column / 2 * width), ((row - j - 1) * height), width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
				}
				else
				{
					//m_scene->addRect(QRect((j * width), ((row / 2) * height), width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
					//m_scene->addRect(QRect(((column - j - 1) * width), ((row / 2) * height), width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
				}
			
			}
			
			evenCells.pop_back();
		}
	}
	else
	{
		//const uint32_t typesCount = m_groupCells.size();
		//for (size_t i = 0; i < typesCount; ++i)
		//{
		//	GroupCells* currentDetail = m_groupCells.back();

		//	height = currentDetail->GetHeight();
		//	width = currentDetail->GetWidth();
		//	Qt::GlobalColor color = GetGlobalColorByType(currentDetail->GetType());

		//	// horizontal
		//	if (row == 1)
		//	{
		//		for (size_t j = 0; j < column; j += typesCount)
		//		{
		//			m_scene->addRect(QRect(((i + j) * width), height, width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
		//		}
		//	}
		//	else // vertical
		//	{
		//		for (size_t j = 0; j < row; j += typesCount)
		//		{
		//			m_scene->addRect(QRect(width, ((i + j) * height), width, height), QPen(Qt::black), QBrush(color, Qt::SolidPattern));
		//		}
		//	}

		//	m_groupCells.pop_back();
		//}
	}
}

void InterDigitation::Route(const std::vector<std::pair<QString, QString>>& ids)
{
}

std::vector<std::pair<QString, QString>> InterDigitation::Parse(QString&& text)
{
	return std::vector<std::pair<QString, QString>>();
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

void InterDigitation::on_ADD_released()
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

void InterDigitation::on_browse_released()
{
	//QString fileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
	//ui->path->setText(fileName);
}

void InterDigitation::on_Place_released()
{
	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to place!");
		return;
	}

	if (ui->CommonCentroid->isChecked())
	{
		SetMode(Mode::CommonCentroid);
	}
	else if (ui->Periodic->isChecked())
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

	ui->graphicsView->setScene(m_scene);
}

void InterDigitation::on_Route_released()
{
}

void InterDigitation::onInterDigitationChosen()
{
	show();
}
