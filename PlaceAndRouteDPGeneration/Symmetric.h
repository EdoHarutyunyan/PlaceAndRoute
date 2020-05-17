#pragma once

#include "IPlaceDesign.h"
#include "Router.h"
#include "Parser.h"

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class QPushButton;
class QComboBox;
class QRadioButton;
class QLineEdit;
class GroupCellsModel;

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
	std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() override;

	void SetSymmetryLine(SymmetryLine symmetryLine);
	SymmetryLine GetSymmetryLine() const;

private:
	void Initialize();
	void SetStyleSheets();
	void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height);
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
	std::shared_ptr<Parser> m_parser;
	std::shared_ptr<Router> m_router;
	GroupCellsModel* m_groupCellsModel;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_scene;
	QPushButton* m_backButton;
	QPushButton* m_detailsButton;
	QPushButton* m_addButton;
	QPushButton* m_browseButton;
	QPushButton* m_placeButton;
	QPushButton* m_routeButton;
	QComboBox* m_typeComboBox;
	QRadioButton* m_horizontalRadioButton;
	QRadioButton* m_verticalRadioButton;
	QLineEdit* m_countLineEdit;
	QLineEdit* m_widthLineEdit;
	QLineEdit* m_heightLineEdit;
	QLineEdit* m_browseLineEdit;
};