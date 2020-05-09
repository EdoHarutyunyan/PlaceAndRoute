#pragma once
#include <vector>
#include <QString>
#include <QGraphicsLineItem>

class IRoute
{
public:
	virtual QVector<QGraphicsLineItem*> Route(
		const std::vector<std::vector<uint32_t>>& idsAdj,
		const QList<QGraphicsItem*>& items,
		const uint32_t width,
		const uint32_t height) = 0;
};