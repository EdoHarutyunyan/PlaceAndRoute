#ifndef MINIMUMAREA_H
#define MINIMUMAREA_H

#include <QWidget>

namespace Ui {
class MinimumArea;
}

class MinimumArea : public QWidget
{
    Q_OBJECT

public:
    explicit MinimumArea(QWidget *parent = nullptr);
    ~MinimumArea();

public slots:
    void onMinimumAreaChosen();

private:
    Ui::MinimumArea *ui;
};

#endif // MINIMUMAREA_H
