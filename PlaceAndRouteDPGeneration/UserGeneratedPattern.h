#pragma once
#include "IPlaceDesign.h"
#include "IRoute.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QPixmap>

class UserGeneratedPattern : public QWidget, public IPlaceDesign, public IRoute
{
	Q_OBJECT

public:
	explicit UserGeneratedPattern(QWidget* parent = nullptr);
	~UserGeneratedPattern() = default;

	void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) override;
	void Route(const std::vector<std::vector<uint32_t>>& idsAdj) override;
	std::vector<std::vector<uint32_t>> Parse(QString&& text) override;
	std::vector<std::pair<uint32_t, uint32_t>> AreaGeneration() override;

private:
	void Initialize();
	void SetStyleSheets();
	QString GetPatternPicPath(const QString& boxText);
	Qt::GlobalColor GetColorByText(const QString& boxText);
	Qt::GlobalColor GetColorByIndex(uint32_t i, uint32_t j);

public slots:
	void onUserGeneratedPatternChosen();

private slots:
	void on_Back_released();
	void on_Browse_released();
	void on_Details_released();
	void on_Place_released();
	void on_Route_released();

private:
	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_scene;
	QPushButton* m_backButton;
	QPushButton* m_detailsButton;
	QPushButton* m_browseButton;
	QPushButton* m_placeButton;
	QPushButton* m_routeButton;
	QLineEdit* m_browseLineEdit;
	QLineEdit* m_rowFactorLineEdit;
	QLineEdit* m_columnFactorLineEdit;
	QLabel* m_rowFactorLabel;
	QLabel* m_columnFactorLabel;
	QComboBox* m_topLeftComboBox;
	QComboBox* m_topRightComboBox;
	QComboBox* m_bottomLeftComboBox;
	QComboBox* m_bottomRightComboBox;
	QLabel* m_topLeftLabel;
	QLabel* m_topRightLabel;
	QLabel* m_bottomLeftLabel;
	QLabel* m_bottomRightLabel;
	QPixmap m_topLeftPic;
	QPixmap m_topRightPic;
	QPixmap m_bottomLeftPic;
	QPixmap m_bottomRightPic;
};
