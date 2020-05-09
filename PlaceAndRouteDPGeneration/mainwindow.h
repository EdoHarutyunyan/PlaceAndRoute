#pragma once

#include <QMainWindow>
#include <QPushButton>

class Symmetric;
class InterDigitation;
class UserGeneratedPattern;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	~MainWindow() = default;

private slots:
    void on_symmetric_released();
	void on_interDegitation_released();
    void on_userGeneratedPattern_released();

private:
	void Initialize();

private:
    Symmetric* m_symmetric;
    InterDigitation* m_interDigitation;
	UserGeneratedPattern* m_userGeneratedPattern;

	QPushButton* m_symmetricButton;
	QPushButton* m_interDigitationButton;
	QPushButton* m_userGeneratedPatternButton;
};
