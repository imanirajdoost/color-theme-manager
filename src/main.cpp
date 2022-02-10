#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "colorpair.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

    //std::cout << "testing" << std::endl;
    ColorPair col;
    std::cout << col.toRGBA(QColor("aaaaaa")).toStdString() << std::endl;


	return a.exec();
}
