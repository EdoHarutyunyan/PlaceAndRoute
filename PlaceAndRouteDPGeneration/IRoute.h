#pragma once
#include <vector>
#include <QString>

class IRoute
{
public:
	virtual void Route(const std::vector<std::vector<uint32_t>>& idsAdj) = 0;
	virtual std::vector<std::vector<uint32_t>> Parse(QString&& text) = 0;
};