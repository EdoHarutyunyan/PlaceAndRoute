#include "Cell.h"

Cell::Cell(const Type type, const uint32_t width, const uint32_t height, const uint32_t id)
	: m_type(type)
	, m_width(width)
	, m_height(height)
	, m_id(id)
{
}

void Cell::SetType(const Type type)
{
	m_type = type;
}

void Cell::SetWidth(const uint32_t width)
{
	m_width = width;
}

inline void Cell::SetHeight(const uint32_t height)
{
	m_height = height;
}

void Cell::SetId(const uint32_t id)
{
	m_id = id;
}

Cell::Type Cell::GetType() const
{
	return m_type;
}

uint32_t Cell::GetWidth() const
{
	return m_width;
}

uint32_t Cell::GetHeight() const
{
	return m_height;
}

uint32_t Cell::GetId() const
{
	return m_id;
}
