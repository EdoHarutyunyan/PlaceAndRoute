#include "Parser.h"
#include "GraphUtils.h"
#include <sstream>

std::vector<std::vector<uint32_t>> Parser::Parse(QString&& text)
{
	std::vector<std::vector<uint32_t>> idsAdj;
	idsAdj.resize(1000);
	std::vector<std::string> tokens;
	std::stringstream stream(text.toStdString());
	std::string item;

	while (std::getline(stream, item, ' '))
	{
		if (item != "-")
		{
			tokens.push_back(item);
		}
	}

	for (size_t i = 0; i <= tokens.size() - 2; i += 2)
	{
		uint32_t source = graphUtils::GetIdNumberByString(tokens[i]);
		uint32_t target = graphUtils::GetIdNumberByString(tokens[i + 1]);

		graphUtils::ConnectNodes(idsAdj, source, target);
	}

	return idsAdj;
}
