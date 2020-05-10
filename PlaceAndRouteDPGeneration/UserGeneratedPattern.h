#pragma once
#include "IPlaceDesign.h"
#include "Router.h"
#include "Parser.h"

#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class QPushButton;
class QLabel;
class QComboBox;
class QRadioButton;
class QLineEdit;

class UserGeneratedPattern : public QWidget, public IPlaceDesign
{
	Q_OBJECT

public:
	explicit UserGeneratedPattern(QWidget* parent = nullptr);
	~UserGeneratedPattern() = default;

	void Place(uint32_t row, uint32_t column, QGraphicsRectItem* area) override;
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
	std::shared_ptr<Parser> m_parser;
	std::shared_ptr<Router> m_router;

	QGraphicsView* m_graphicsView;
	QGraphicsScene* m_scene;
	QPushButton* m_backButton;
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
