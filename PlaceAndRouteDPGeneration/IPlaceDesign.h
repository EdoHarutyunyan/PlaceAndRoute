#pragma once

#include <vector>
#include "GroupCells.h"
#include <QGraphicsRectItem>

class IPlaceDesign
{
public:
	virtual void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) = 0;
	virtual std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() = 0;

protected:
	GroupCells m_groupCells;
};
