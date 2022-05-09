#include "colorwidget.h"
#include "ui_colorwidget.h"

#include <iostream>

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorWidget)
{
    ui->setupUi(this);
}

ColorWidget::~ColorWidget()
{
    delete ui;
}

void ColorWidget::setColorRef(ColorPair* c)
{
    currentColor = c;
    //std::cout << "color  = " << currentColor.toRGBA(currentColor.getColorSource()).toStdString() << std::endl;
    ui->colorSourceText->setPlainText(ColorPair::toRGBA(currentColor->getColorSource()));
    ui->colorDestText->setPlainText(ColorPair::toRGBA(currentColor->getColorTarget()));
}
