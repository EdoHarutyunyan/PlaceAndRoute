#pragma once

#include <vector>
#include "GroupCells.h"
#include <QGraphicsRectItem>

class IPlaceDesign
{
public:
	virtual void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) = 0;
	virtual void Route(const std::vector<std::vector<uint32_t>>& idsAdj) = 0;
	virtual std::vector<std::vector<uint32_t>> Parse(QString&& text) = 0;
	virtual std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() = 0;

private:
	virtual void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height) = 0;
	// IDetails* GetEvenRandomDetail() const;

protected:
	GroupCells m_groupCells;
};
