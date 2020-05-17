#pragma once
#include "Cell.h"

#include <QAbstractTableModel>
#include <QString>
#include <QVector>

class GroupCellsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	GroupCellsModel(QObject* parent = nullptr);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void UpdateTableData(Cell::Type type, uint32_t count, uint32_t width, uint32_t height);
	
private:
	QVector<QVector<QString>> m_tableData;
};
