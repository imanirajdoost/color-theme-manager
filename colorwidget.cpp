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
    ui->colorSourceText->setPlainText(ColorPair::toRGBA(currentColor->getColorSource()));
    ui->colorDestText->setPlainText(ColorPair::toRGBA(currentColor->getColorTarget()));
    ui->colorIdLabel->setText(currentColor->getId());

    updateSource();
    updateDestination();
}

void ColorWidget::updateSource() {
    QString col = ColorPair::toRGBA(currentColor->getColorSource());
    ui->colorSourceButton->setStyleSheet(QString("background-color:" + col + ";"));
}

void ColorWidget::updateDestination() {
    QString col = ColorPair::toRGBA(currentColor->getColorTarget());
    ui->colorDestButton->setStyleSheet(QString("background-color:" + col + ";"));
}

void ColorWidget::on_colorSourceButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->open(this, SLOT(changeSourceColor(QColor)));
}

void ColorWidget::changeSourceColor(QColor sourceCol) {
    currentColor->setColorSource(sourceCol);
    ui->colorSourceText->setPlainText(ColorPair::toRGBA(currentColor->getColorSource()));
    updateSource();
}

void ColorWidget::changeDestinationColor(QColor destCol) {
    currentColor->setColorTarget(destCol);
    ui->colorDestText->setPlainText(ColorPair::toRGBA(currentColor->getColorTarget()));
    updateDestination();
}


void ColorWidget::on_colorDestButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->open(this, SLOT(changeDestinationColor(QColor)));
}

