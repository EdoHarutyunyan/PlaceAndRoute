#include "GroupCells.h"

GroupCells::GroupCells()
	: m_count(0U)
{
}

void GroupCells::AddCells(const std::vector<Cell>& cells)
{
	const Cell::Type type = cells[0].GetType();
	auto it = m_cells.find(type);
	
	if (it != m_cells.end())
	{
		it->second.insert(it->second.begin(), cells.begin(), cells.end());
	}
	else
	{
		m_cells.insert({ type, cells });
	}

	m_count += cells.size();
}

void GroupCells::SetCount(const uint32_t count)
{
	m_count = count;
}

uint32_t GroupCells::GetCount() const
{
	return m_count;
}

std::map<Cell::Type, std::vector<Cell>> GroupCells::GetCells() const
{
	return m_cells;
}
