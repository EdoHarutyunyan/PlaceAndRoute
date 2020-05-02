#pragma once
#include <cstdint>

class Cell
{
public:
	enum class Type : uint32_t
	{
		Green = 0,
		Yellow = 1,
		Blue = 2,
		Red = 3
	};

	Cell() = default;
	Cell(Type type, uint32_t width, uint32_t height, uint32_t id);

	// Setters
	void SetType(Type type);
	void SetWidth(uint32_t width);
	void SetHeight(uint32_t height);
	void SetId(uint32_t id);

	// Getters
	Type GetType() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	uint32_t GetId() const;

private:
	Type m_type;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_id;
};