#include "mainwindow.h"
#include <QSplashScreen>
#include <QTimer>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QPixmap pixmap(":/PlaceAndRouteDPGeneration/Resources/SplashScreen.png");
	//QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
	//splash.show();
	//QTimer::singleShot(5000, &splash, &QWidget::close); // keep displayed for 5 seconds
	MainWindow w;
	w.show();
	return a.exec();
}
