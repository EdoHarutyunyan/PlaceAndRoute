#ifndef Symmetric_H
#define Symmetric_H

#include "IPlaceDesign.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>

class Symmetric : public QWidget, public IPlaceDesign
{
    Q_OBJECT

public:
    explicit Symmetric(QWidget *parent = nullptr);
	~Symmetric() = default;

	enum class SymmetryLine : bool
	{
		Horizontal = 0,
		Vertical = 1
	};

	void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) override;
	void Route(const std::vector<std::vector<uint32_t>>& idsAdj) override;
	std::vector<std::vector<uint32_t>> Parse(QString&& text) override;
	std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() override;

	void SetSymmetryLine(SymmetryLine symmetryLine);
	SymmetryLine GetSymmetryLine() const;

private:
	void Initialize();
	void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height) override;
	Qt::GlobalColor GetGlobalColorByType(Cell::Type type) const;

public slots:
    void onSymmetricChosen();

private slots:
	void on_Add_released();
	void on_Back_released();
	void on_Browse_released();
	void on_Details_released();
	void on_Place_released();
	void on_Route_released();

private:
	SymmetryLine m_symmetryLine;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_scene;
	QPushButton* m_backButton;
	QPushButton* m_detailsButton;
	QPushButton* m_addButton;
	QPushButton* m_browseButton;
	QPushButton* m_placeButton;
	QPushButton* m_routeButton;
	QLabel* m_typeLabel;
	QLabel* m_countLabel;
	QLabel* m_widthLabel;
	QLabel* m_heightLabel;
	QComboBox* m_typeComboBox;
	QRadioButton* m_horizontalRadioButton;
	QRadioButton* m_verticalRadioButton;
	QLineEdit* m_countLineEdit;
	QLineEdit* m_widthLineEdit;
	QLineEdit* m_heightLineEdit;
	QLineEdit* m_browseLineEdit;
};

#endif // Symmetric_H
