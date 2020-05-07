#include "Symmetric.h"

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cassert>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsSimpleTextItem>
#include <QGridLayout>

#include <QLine>
#include <QSpacerItem>

Symmetric::Symmetric(QWidget *parent)
	: QWidget(parent)
	, m_graphicsView(new QGraphicsView(this))
	, m_scene(new QGraphicsScene(this))
	, m_backButton(new QPushButton("< Back", this))
	, m_detailsButton(new QPushButton(this))
	, m_addButton(new QPushButton("Add", this))
	, m_browseButton(new QPushButton("Browse", this))
	, m_placeButton(new QPushButton("Place", this))
	, m_routeButton(new QPushButton("Route", this))
	, m_typeLabel(new QLabel("Type", this))
	, m_countLabel(new QLabel("Count", this))
	, m_widthLabel(new QLabel("Width", this))
	, m_heightLabel(new QLabel("Height", this))
	, m_typeComboBox(new QComboBox(this))
	, m_horizontalRadioButton(new QRadioButton("Horizontal", this))
	, m_verticalRadioButton(new QRadioButton("Vertical", this))
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

uint32_t GetIdNumberByString(const std::string& idString)
{
	std::stringstream idStream(idString);
	uint32_t idNumber{};
	char first;
	char second;

	idStream >> first;
	idStream >> second;
	idStream >> idNumber;

	return idNumber;
}

QString GetIdStringByNumber(const uint32_t idNumber)
{
	std::string idString = "Id" + std::to_string(idNumber);

	return QString::fromStdString(idString);
}

void connectNodes(std::vector<std::vector<uint32_t>>& graph, const int source, const int target)
{
	for (size_t i = 0; i < graph[source].size(); ++i)
	{
		if (target == graph[source][i])
		{
			return;
		}
	}
	for (size_t i = 0; i < graph[target].size(); ++i)
	{
		if (target == graph[target][i])
		{
			return;
		}
	}

	graph[source].push_back(target);
	graph[target].push_back(source);
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

std::vector<std::vector<uint32_t>> Symmetric::Parse(QString&& text)
{
	std::vector<std::vector<uint32_t>> idsAdj;
	idsAdj.resize(100);
	std::vector<std::string> tokens;
	std::stringstream stream(text.toStdString());
	std::string item;

	while (std::getline(stream, item, ' '))
	{
		if (item != "-")
		{
			tokens.push_back(item);
		}
	}

	if (tokens.size() > 2)
	{
		for (size_t i = 0; i < tokens.size() - 2; i += 2)
		{
			uint32_t source = GetIdNumberByString(tokens[i]);
			uint32_t target = GetIdNumberByString(tokens[i + 1]);

			connectNodes(idsAdj, source, target);
		}
	}
	else
	{
		uint32_t source = GetIdNumberByString(tokens[0]);
		uint32_t target = GetIdNumberByString(tokens[1]);

		connectNodes(idsAdj, source, target);
	}
	return idsAdj;
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
			if (item != "-")
			{
				tokens.push_back(item);
			}
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

void Symmetric::Initialize()
{
	QGridLayout* mainLayout = new QGridLayout();

	QFrame* line = new QFrame();
	line->setGeometry(QRect(/* ... */));
	line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
	line->setFrameShadow(QFrame::Sunken);

	QLabel* title = new QLabel("SYMMETRIC", this);
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
	mainLayout->addWidget(m_typeLabel, row, 0, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(m_countLabel, row, 1, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(m_widthLabel, row, 2, 1, 1, Qt::AlignCenter);
	mainLayout->addWidget(m_heightLabel, row, 3, 1, 1, Qt::AlignCenter);
	mainLayout->addItem(new QSpacerItem(25, 25), row, 4, 1, 1);
	++row;
	// row4
	mainLayout->addWidget(m_typeComboBox, row, 0, 1, 1);
	mainLayout->addWidget(m_countLineEdit, row, 1, 1, 1);
	mainLayout->addWidget(m_widthLineEdit, row, 2, 1, 1);
	mainLayout->addWidget(m_heightLineEdit, row, 3, 1, 1);
	mainLayout->addWidget(m_addButton, row, 4, 1, 1);
	++row;
	// row5, 6
	mainLayout->addWidget(m_horizontalRadioButton, row, 1, 1, 1);
	mainLayout->addWidget(m_browseButton, row, 2, 2, 1, Qt::AlignRight);
	mainLayout->addWidget(m_browseLineEdit, row, 3, 2, 1);
	mainLayout->addWidget(m_verticalRadioButton, ++row, 1, 1, 1);
	++row;
	// row7-14
	mainLayout->addWidget(m_graphicsView, row, 0, 8, 4);
	mainLayout->addWidget(m_placeButton, 13, 4, 1, 1);
	mainLayout->addWidget(m_routeButton, 14, 4, 1, 1);

	setLayout(mainLayout);

	showMaximized();
	setWindowTitle("Symmetric");

	// Initilize ComboBox 
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/green.png"), "green");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/yellow.png"), "yellow");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/blue.jpg"), "blue");
	m_typeComboBox->addItem(QIcon(":/PlaceAndRouteDPGeneration/Resources/red.jpg"), "red");

	// Set labels alignments
	m_countLineEdit->setAlignment(Qt::AlignRight);
	m_widthLineEdit->setAlignment(Qt::AlignRight);
	m_heightLineEdit->setAlignment(Qt::AlignRight);

	SetStyleSheets();
}

void Symmetric::SetStyleSheets()
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

void Symmetric::on_Add_released()
{
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

void Symmetric::on_Place_released()
{
	if (!m_groupCells.GetCount())
	{
		QMessageBox::warning(this, "Warning!", "Not elements to place!");
		return;
	}

	if (m_verticalRadioButton->isChecked())
	{
		SetSymmetryLine(SymmetryLine::Vertical);
	}
	else if (m_horizontalRadioButton->isChecked())
	{
		SetSymmetryLine(SymmetryLine::Horizontal);
	}
	else
	{
		QMessageBox::warning(this, "Warning!", "Please choose symmetry line");
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
};

void Symmetric::on_Back_released()
{
	close();
	// show mainwindow // TODO
}

void Symmetric::on_Browse_released()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open a file", QDir::homePath());
	m_browseLineEdit->setText(fileName);
}

void Symmetric::on_Details_released()
{
}

void Symmetric::on_Route_released()
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

	std::vector<std::vector<uint32_t>> idsAdj = Parse(std::move(text));

	Route(idsAdj);
}


bool BFS(
	const std::vector<std::vector<uint32_t>>& graph,
	const uint32_t source,
	const uint32_t target,
	std::vector<uint32_t>& predecessor,
	std::vector<uint32_t>& distance)
{
	std::list<int> queue;
	std::vector<bool> visited;
	visited.resize(graph.size());

	for (int i = 0; i < graph.size(); i++)
	{
		visited[i] = false;
		distance[i] = INT_MAX;
		predecessor[i] = -1;
	}

	visited[source] = true;
	distance[source] = 0;
	queue.push_back(source);

	// standard BFS algorithm 
	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (int i = 0; i < graph[u].size(); i++)
		{
			if (visited[graph[u][i]] == false)
			{
				visited[graph[u][i]] = true;
				distance[graph[u][i]] = distance[u] + 1;
				predecessor[graph[u][i]] = u;
				queue.push_back(graph[u][i]);

				if (graph[u][i] == target)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void Symmetric::Route(const std::vector<std::vector<uint32_t>>& idsAdj)
{
	QList<QGraphicsItem*> items = m_scene->items();
	QList<QGraphicsSimpleTextItem*> itemIds;
	std::vector<uint32_t> predecessor;
	std::vector<uint32_t> distance;
	predecessor.resize(idsAdj.size());
	distance.resize(idsAdj.size());

	auto cells = m_groupCells.GetCells();
	auto it = cells.begin();

	const uint32_t height = it->second.back().GetHeight();
	const uint32_t width = it->second.back().GetWidth();

	for (QGraphicsItem* item : items)
	{
		QGraphicsSimpleTextItem* id = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);

		if (id)
		{
			itemIds.push_back(id);
		}
	}

	std::vector<std::vector<uint32_t>> graph;
	graph.resize(itemIds.size());

	QVector<QPointF> parentCenters;
	parentCenters.resize(itemIds.size());

	for (size_t i = 0; i < itemIds.size(); ++i)
	{
		auto currentParent = itemIds[i]->parentItem();

		QGraphicsRectItem* currentParentRect = qgraphicsitem_cast<QGraphicsRectItem*>(currentParent);

		if (currentParentRect == nullptr)
		{
			return;
		}

		QPointF currentPoint = currentParent->pos();

		parentCenters[i] = currentPoint;

		QPointF candidatePoint1(currentPoint.x() + width, currentPoint.y());
		QPointF candidatePoint2(currentPoint.x() - width, currentPoint.y());
		QPointF candidatePoint3(currentPoint.x(), currentPoint.y() + height);
		QPointF candidatePoint4(currentPoint.x(), currentPoint.y() - height);

		for (size_t j = 0; j < itemIds.size(); ++j)
		{
			if (i != j)
			{
				auto otherParent = itemIds[j]->parentItem();

				QGraphicsRectItem* otherParentRect = dynamic_cast<QGraphicsRectItem*>(otherParent);

				if (otherParentRect == nullptr)
				{
					return;
				}

				QPointF neighborPoint(otherParentRect->pos());

				if (neighborPoint == candidatePoint1 ||
					neighborPoint == candidatePoint2 ||
					neighborPoint == candidatePoint3 ||
					neighborPoint == candidatePoint4)
				{
					uint32_t source = GetIdNumberByString(itemIds[i]->text().toStdString());
					uint32_t target = GetIdNumberByString(itemIds[j]->text().toStdString());
					connectNodes(graph, source, target);
				}
			}

		}
	}

	for (size_t i = 0; i < idsAdj.size(); ++i)
	{
		for (size_t j = 1; j <= idsAdj[i].size(); ++j)
		{
			if (!BFS(graph, i, idsAdj[i][j - 1], predecessor, distance))
			{
				// QMessageBox::warning(this, "Warning!", "Not elements to route!");
				return;
			}

			std::vector<uint32_t> path;
			int crawl = idsAdj[i][j - 1];
			path.push_back(crawl);

			while (predecessor[crawl] != -1)
			{
				path.push_back(predecessor[crawl]);
				crawl = predecessor[crawl];
			}

			for (size_t p = 0; p < path.size() - 1; ++p)
			{
				QPointF sourcePoint(0, 0);
				QPointF targetPoint(0, 0);

				QString idSource = GetIdStringByNumber(path[p]);
				QString idTarget = GetIdStringByNumber(path[p + 1]);

				for (size_t i = 0; i < itemIds.size(); ++i)
				{
					if (itemIds[i]->text() == idSource)
					{
						sourcePoint = parentCenters[i];
					}
					else if (itemIds[i]->text() == idTarget)
					{
						targetPoint = parentCenters[i];
					}
				}

				QLineF line(sourcePoint, targetPoint);
				m_scene->addLine(line, QPen(Qt::red));
			}

			path.clear();
			distance.clear();
			predecessor.clear();
			break;
		}
		break;
	}

	m_graphicsView->setScene(m_scene);
}
