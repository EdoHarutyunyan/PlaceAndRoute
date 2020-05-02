#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Symmetric;
class InterDigitation;
class MinimumArea;
class Temperature;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void SymmetricChosen();
    void InterDigitationChosen();
    void MinimumAreaChosen();
    void TemperatureChosen();

private slots:
    void on_symmetric_released();
    void on_interDegitation_released();
    void on_minimumArea_released();
    void on_temperature_released();

private:
    Ui::MainWindow *ui;

    Symmetric* m_symmetric;
    InterDigitation* m_interDigitation;
    MinimumArea* m_minimumArea;
    Temperature* m_temperature;
};
#endif // MAINWINDOW_H
