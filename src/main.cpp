#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "colorpair.h"
#include "xmlreader.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

    /*
    ColorPair col;
    std::cout << col.toRGBA(QColor("aaaaaa")).toStdString() << std::endl;

    XMLReader reader;
    QFile file;
    file.setFileName("darkTheme.xml");
    QDir::setCurrent("D:/");

    reader.read(file);
    */


	return a.exec();
}
