#pragma once
#include "IRoute.h"
#include <vector>
#include <QGraphicsRectItem>

class Router : public IRoute
{
public:
	Router() = default;

	QVector<QGraphicsLineItem*> Route(
		const std::vector<std::vector<uint32_t>>& idsAdj,
		const QList<QGraphicsItem*>& items,
		const uint32_t width,
		const uint32_t height) override;
private:
	bool BFS(
		const std::vector<std::vector<uint32_t>>& graph,
		const uint32_t source,
		const uint32_t target,
		std::vector<uint32_t>& predecessor,
		std::vector<uint32_t>& distance);

	void PrepareGraph(
		std::vector<std::vector<uint32_t>>& graph,
		QVector<QGraphicsRectItem*>& parentRects,
		const QList<QGraphicsSimpleTextItem*>& itemIds,
		uint32_t width,
		uint32_t height);
};

