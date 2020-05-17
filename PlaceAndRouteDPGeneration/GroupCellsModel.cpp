#include "GroupCellsModel.h"
#include <QColor>

GroupCellsModel::GroupCellsModel(QObject* parent)
	: QAbstractTableModel(parent)
{
}

int GroupCellsModel::rowCount(const QModelIndex& /*parent*/) const
{
	return m_tableData.size();
}

int GroupCellsModel::columnCount(const QModelIndex& /*parent*/) const
{
	return 4;
}

QVariant GroupCellsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::BackgroundRole)
		{
			return QColor(204, 105, 0);
		}

		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
			case 0:
				return QString("Type");
			case 1:
				return QString("Count");
			case 2:
				return QString("Width");
			case 3:
				return QString("Height");
			}
		}
	}

	return QVariant();
}

QVariant GroupCellsModel::data(const QModelIndex& index, int role) const
{
	const uint32_t row = index.row();
	const uint32_t column = index.column();

	if (role == Qt::BackgroundRole)
	{
		if (m_tableData[row][column] == "Green")
		{
			return QColor(Qt::green);
		}
		if (m_tableData[row][column] == "Yellow")
		{
			return QColor(Qt::yellow);
		}
		if (m_tableData[row][column] == "Blue")
		{
			return QColor(Qt::blue);
		}
		if (m_tableData[row][column] == "Red")
		{
			return QColor(Qt::red);
		}
	}

	if (role == Qt::DisplayRole)
	{
		return m_tableData[row][column];
	}

	return QVariant();
}

void GroupCellsModel::UpdateTableData(
	const Cell::Type type,
	const uint32_t count, 
	const uint32_t width, 
	const uint32_t height)
{
	std::string typeString;

	switch (type)
	{
	case Cell::Type::Green:
		typeString = "Green";
		break;
	case Cell::Type::Yellow:
		typeString = "Yellow";
		break;
	case Cell::Type::Blue:
		typeString = "Blue";
		break;
	case Cell::Type::Red:
		typeString = "Red";
		break;
	default:
		break;
	}

	for (size_t i = 0; i < m_tableData.size(); ++i)
	{
		if (QString::fromStdString(typeString) == m_tableData[i][0])
		{
			m_tableData[i][1] = QString::number(m_tableData[i][1].toUInt() + count);
			return;
		}
	}

	QVector<QString> newRowData;
	newRowData.push_back(QString::fromStdString(typeString));
	newRowData.push_back(QString::number(count));
	newRowData.push_back(QString::number(width));
	newRowData.push_back(QString::number(height));

	m_tableData.push_back(newRowData);
}
