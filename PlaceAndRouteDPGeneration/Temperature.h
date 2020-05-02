#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QWidget>

namespace Ui {
class Temperature;
}

class Temperature : public QWidget
{
    Q_OBJECT

public:
    explicit Temperature(QWidget *parent = nullptr);
    ~Temperature();

public slots:
    void onTemperatureChosen();

private:
    Ui::Temperature *ui;
};

#endif // TEMPERATURE_H
