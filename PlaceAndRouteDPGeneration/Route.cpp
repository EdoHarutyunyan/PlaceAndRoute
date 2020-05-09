//#include "Route.h"
//#include <list>
//
//bool Route::BFS(
//	const std::vector<std::vector<uint32_t>>& graph,
//	const uint32_t source,
//	const uint32_t target,
//	std::vector<uint32_t>& predecessor,
//	std::vector<uint32_t>& distance)
//{
//	std::list<int> queue;
//	std::vector<bool> visited;
//
//	for (size_t i = 0; i < graph.size(); ++i)
//	{
//		visited.push_back(false);
//		distance.push_back(INT_MAX);
//		predecessor.push_back(-1);
//	}
//
//	visited[source] = true;
//	distance[source] = 0;
//	queue.push_back(source);
//
//	// standard BFS algorithm 
//	while (!queue.empty())
//	{
//		int u = queue.front();
//		queue.pop_front();
//		for (size_t i = 0; i < graph[u].size(); i++)
//		{
//			if (visited[graph[u][i]] == false)
//			{
//				visited[graph[u][i]] = true;
//				distance[graph[u][i]] = distance[u] + 1;
//				predecessor[graph[u][i]] = u;
//				queue.push_back(graph[u][i]);
//
//				if (graph[u][i] == target)
//				{
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
//
////std::vector<std::string> Tokenize(std::string&& line)
////{
////	std::vector<std::string> tokens;
////	std::stringstream stream(line);
////	std::string item;
////
////	while (std::getline(stream, item, ' '))
////	{
////		if (!item.empty())
////		{
////			if (item != "-")
////			{
////				tokens.push_back(item);
////			}
////		}
////	}
////
////	return tokens;
////}
////
//
//
//void Route::Route(const std::vector<std::vector<uint32_t>>& idsAdj)
//{
//}
//
//std::vector<std::vector<uint32_t>> Route::Parse(QString&& text)
//{
//	return std::vector<std::vector<uint32_t>>();
//}
