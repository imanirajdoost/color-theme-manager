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

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent = nullptr);
    ~ColorWidget();

private:
    Ui::ColorWidget *ui;
};

#endif // COLORWIDGET_H
