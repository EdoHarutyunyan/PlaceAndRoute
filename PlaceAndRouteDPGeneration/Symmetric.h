#ifndef Symmetric_H
#define Symmetric_H

#include <QWidget>
#include <QGraphicsScene>
#include "IPlaceDesign.h"

namespace Ui {
class Symmetric;
}

class Symmetric : public QWidget, public IPlaceDesign
{
    Q_OBJECT

public:
    explicit Symmetric(QWidget *parent = nullptr);
    ~Symmetric();

	enum class SymmetryLine : bool
	{
		Horizontal = 0,
		Vertical = 1
	};

	void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) override;
	void Route(const std::vector<std::pair<QString, QString>>& ids) override;
	std::vector<std::pair<QString, QString>> Parse(QString&& text) override;
	std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() override;

	void SetSymmetryLine(SymmetryLine symmetryLine);
	SymmetryLine GetSymmetryLine() const;

private:
	void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height) override;
	Qt::GlobalColor GetGlobalColorByType(Cell::Type type) const;

public slots:
    void onSymmetricChosen();

private slots:
	void on_browse_released();
	void on_ADD_released();
	void on_Place_released();
	void on_Route_released();

private:
    Ui::Symmetric *ui;
	QGraphicsScene* m_scene;
	SymmetryLine m_symmetryLine;
};

#endif // Symmetric_H
