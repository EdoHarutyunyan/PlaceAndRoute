#pragma once
#include <QString>
#include <vector>

class Parser
{
public:
	Parser() = default;
	std::vector<std::vector<uint32_t>> Parse(QString&& text);
};

