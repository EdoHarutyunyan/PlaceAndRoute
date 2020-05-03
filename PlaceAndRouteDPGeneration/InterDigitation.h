#ifndef INTERDIGITATION_H
#define INTERDIGITATION_H

#include "IPlaceDesign.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>

class InterDigitation : public QWidget, public IPlaceDesign
{
    Q_OBJECT

public:
    explicit InterDigitation(QWidget *parent = nullptr);
	~InterDigitation() = default;

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
	void Initialize();
	void AddGroupCells(Cell::Type type, uint32_t count, uint32_t width, uint32_t height) override;
	Qt::GlobalColor GetGlobalColorByType(Cell::Type type) const;

public slots:
	void onInterDigitationChosen();

private slots:
	void on_Add_released();
	void on_Back_released();
	void on_Browse_released();
	void on_Details_released();
	void on_Place_released();
	void on_Route_released();

private:
	Mode m_mode;

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
	QRadioButton* m_commonCentroidRadioButton;
	QRadioButton* m_periodicRadioButton;
	QLineEdit* m_countLineEdit;
	QLineEdit* m_widthLineEdit;
	QLineEdit* m_heightLineEdit;
	QLineEdit* m_browseLineEdit;
};

#endif // INTERDIGITATION_H
