#pragma once

#include <vector>
#include <map>
#include "Cell.h"

class GroupCells
{
public:
	GroupCells();

	void AddCells(const std::vector<Cell>& cells);

	void SetCount(uint32_t count);

	uint32_t GetCount() const;
	std::map<Cell::Type, std::vector<Cell>> GetCells() const;

private:
	uint32_t m_count;
	std::map<Cell::Type, std::vector<Cell>> m_cells;
};