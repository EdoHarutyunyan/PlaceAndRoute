#ifndef INTERDIGITATION_H
#define INTERDIGITATION_H

#include <QWidget>
#include <QGraphicsScene>
#include "IPlaceDesign.h"

namespace Ui {
class InterDigitation;
}

class InterDigitation : public QWidget, public IPlaceDesign
{
    Q_OBJECT

public:
    explicit InterDigitation(QWidget *parent = nullptr);
    ~InterDigitation();

	enum class Mode : bool
	{
		CommonCentroid = 0,
		Periodic = 1
	};

	void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) override;
	void Route(const std::vector<std::pair<QString, QString>>& ids) override;
	std::vector<std::pair<QString, QString>> Parse(QString&& text) override;
	std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() override;

	void SetMode(Mode mode);
	Mode GetMode() const;

private:
	void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height) override;
	Qt::GlobalColor GetGlobalColorByType(Cell::Type type) const;

public slots:
	void onInterDigitationChosen();

private slots:
	void on_browse_released();
	void on_ADD_released();
	void on_Place_released();
	void on_Route_released();

private:
    Ui::InterDigitation *ui;
	QGraphicsScene* m_scene;
	Mode m_mode;
};

#endif // INTERDIGITATION_H
