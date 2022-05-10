#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include "src/colorpair.h"

#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QScrollArea>
#include <QGridLayout>
#include <QColorDialog>

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent = nullptr);
    ~ColorWidget();
    void setColorRef(ColorPair* c);

private slots:
    void on_colorSourceButton_clicked();
    void changeSourceColor(QColor sourceCol);
    void changeDestinationColor(QColor destCol);

    void on_colorDestButton_clicked();


private:
    Ui::ColorWidget *ui;
    ColorPair* currentColor;
    void updateSource();
    void updateDestination();
};

#endif // COLORWIDGET_H
