#include "Router.h"
#include "GraphUtils.h"
#include <list>
#include <cassert>
#include <QPen>
#include <QMessageBox>

QVector<QGraphicsLineItem*> Router::Route(
	const std::vector<std::vector<uint32_t>>& idsAdj, const QList<QGraphicsItem*>& items, const uint32_t width, const uint32_t height)
{
	QList<QGraphicsSimpleTextItem*> itemIds;
	std::vector<uint32_t> predecessor;
	std::vector<uint32_t> distance;

	for (QGraphicsItem* item : items)
	{
		QGraphicsSimpleTextItem* id = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);

		if (id)
		{
			itemIds.push_back(id);
		}
	}

	std::vector<std::vector<uint32_t>> graph;
	graph.resize(itemIds.size());

	QVector<QGraphicsRectItem*> parentRects;
	parentRects.resize(itemIds.size());

	PrepareGraph(graph, parentRects, itemIds, width, height);

	QVector<QGraphicsLineItem*> resultLines;

	for (size_t i = 0; i < idsAdj.size(); ++i)
	{
		for (size_t j = 1; j <= idsAdj[i].size(); ++j)
		{
			if (!BFS(graph, i, idsAdj[i][j - 1], predecessor, distance))
			{
				return{};
			}

			std::vector<uint32_t> path;
			int crawl = idsAdj[i][j - 1];
			path.push_back(crawl);

			while (predecessor[crawl] != -1)
			{
				path.push_back(predecessor[crawl]);
				crawl = predecessor[crawl];
			}

			for (size_t p = 0; p < path.size() - 1; ++p)
			{
				QPointF sourcePoint(0, 0);
				QPointF targetPoint(0, 0);

				QString idSource = graphUtils::GetIdStringByNumber(path[p]);
				QString idTarget = graphUtils::GetIdStringByNumber(path[p + 1]);

				QGraphicsItem* lineParent = nullptr;
				for (size_t k = 0; k < itemIds.size(); ++k)
				{
					if (itemIds[k]->text() == idSource)
					{
						sourcePoint = parentRects[k]->pos();
						sourcePoint.rx() += width / 2;
						sourcePoint.ry() += height / 2;
					}
					else if (itemIds[k]->text() == idTarget)
					{
						targetPoint = parentRects[k]->pos();
						targetPoint.rx() += width / 2;
						targetPoint.ry() += height / 2;

						lineParent = parentRects[k]->parentItem();
					}
				}

				QLineF line(sourcePoint, targetPoint);
				QGraphicsLineItem* gLine = new QGraphicsLineItem(line, lineParent);
				gLine->setPen(QPen(Qt::black, 7));

				resultLines.push_back(gLine);
			}

			path.clear();
			distance.clear();
			predecessor.clear();
		}
	}

	return resultLines;
}

void Router::PrepareGraph(
	std::vector<std::vector<uint32_t>>& graph,
	QVector<QGraphicsRectItem*>& parentRects,
	const QList<QGraphicsSimpleTextItem*>& itemIds,
	const uint32_t width, 
	const uint32_t height)
{
	for (size_t i = 0; i < itemIds.size(); ++i)
	{
		auto currentParent = itemIds[i]->parentItem();

		QGraphicsRectItem* currentParentRect = qgraphicsitem_cast<QGraphicsRectItem*>(currentParent);

		if (currentParentRect == nullptr)
		{
			return;
		}

		parentRects[i] = currentParentRect;

		QPointF currentPoint = currentParent->pos();

		QPointF candidatePoint1(currentPoint.x() + width, currentPoint.y());
		QPointF candidatePoint2(currentPoint.x() - width, currentPoint.y());
		QPointF candidatePoint3(currentPoint.x(), currentPoint.y() + height);
		QPointF candidatePoint4(currentPoint.x(), currentPoint.y() - height);

		for (size_t j = 0; j < itemIds.size(); ++j)
		{
			if (i != j)
			{
				auto otherParent = itemIds[j]->parentItem();

				QGraphicsRectItem* otherParentRect = qgraphicsitem_cast<QGraphicsRectItem*>(otherParent);

				if (otherParentRect == nullptr)
				{
					return;
				}

				if (currentParentRect->parentItem()->pos() != otherParentRect->parentItem()->pos())
				{
					continue;
				}

				QPointF neighborPoint(otherParentRect->pos());

				if (neighborPoint == candidatePoint1 ||
					neighborPoint == candidatePoint2 ||
					neighborPoint == candidatePoint3 ||
					neighborPoint == candidatePoint4)
				{
					uint32_t source = graphUtils::GetIdNumberByString(itemIds[i]->text().toStdString());
					uint32_t target = graphUtils::GetIdNumberByString(itemIds[j]->text().toStdString());
					graphUtils::connectNodes(graph, source, target);
				}
			}
		}
	}
}

bool Router::BFS(
	const std::vector<std::vector<uint32_t>>& graph,
	const uint32_t source,
	const uint32_t target,
	std::vector<uint32_t>& predecessor,
	std::vector<uint32_t>& distance)
{
	std::list<int> queue;
	std::vector<bool> visited;

	for (size_t i = 0; i < graph.size(); ++i)
	{
		visited.push_back(false);
		distance.push_back(INT_MAX);
		predecessor.push_back(-1);
	}

	visited[source] = true;
	distance[source] = 0;
	queue.push_back(source);

	// standard BFS algorithm 
	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (size_t i = 0; i < graph[u].size(); i++)
		{
			if (visited[graph[u][i]] == false)
			{
				visited[graph[u][i]] = true;
				distance[graph[u][i]] = distance[u] + 1;
				predecessor[graph[u][i]] = u;
				queue.push_back(graph[u][i]);

				if (graph[u][i] == target)
				{
					return true;
				}
			}
		}
	}

	return false;
}
