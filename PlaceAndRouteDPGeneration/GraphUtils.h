#pragma once
#include <sstream>
#include <QString>

namespace graphUtils
{
	inline uint32_t GetIdNumberByString(const std::string& idString)
	{
		std::stringstream idStream(idString);
		uint32_t idNumber{};
		char first;
		char second;

		idStream >> first;
		idStream >> second;
		idStream >> idNumber;

		return idNumber;
	}

	inline QString GetIdStringByNumber(const uint32_t idNumber)
	{
		std::string idString = "Id" + std::to_string(idNumber);

		return QString::fromStdString(idString);
	}

	inline void ConnectNodes(std::vector<std::vector<uint32_t>>& graph, const int source, const int target)
	{
		if (std::find(graph[source].begin(), graph[source].end(), target) != graph[source].end())
		{
			assert(std::find(graph[target].begin(), graph[target].end(), source) != graph[target].end());
			return;
		}

		graph[source].push_back(target);
	}
}