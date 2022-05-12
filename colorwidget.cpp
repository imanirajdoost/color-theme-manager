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
    //ui->colorIdLabel->setText(currentColor->getId());
    ui->colorIdTextEdit->setText(currentColor->getId());

    updateSource(currentColor->getColorSource());
    updateDestination(currentColor->getColorTarget());
    ui->undoColorSourceButton->setVisible(false);
    ui->undoColorDestButton->setVisible(false);
    ui->undoColorIdButton->setVisible(false);
}

void ColorWidget::updateSource(QColor newColor) {
    QString col = ColorPair::toRGBA(newColor);
    ui->colorSourceButton->setStyleSheet(QString("background-color:" + col + ";"));
}

void ColorWidget::updateDestination(QColor newColor) {
    QString col = ColorPair::toRGBA(newColor);
    ui->colorDestButton->setStyleSheet(QString("background-color:" + col + ";"));
}

void ColorWidget::on_colorSourceButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->open(this, SLOT(changeSourceColor(QColor)));
}

void ColorWidget::changeSourceColor(QColor sourceCol) {
    newSourceColor = sourceCol;
    if(QString::compare(ColorPair::toRGBA(sourceCol),ColorPair::toRGBA(currentColor->getColorSource())))
    {
        isSourceColorChanged = true;
        ui->undoColorSourceButton->setVisible(true);
        // Make the button "visible"
        //        ui->undoColorSourceButton->setPalette(QPalette()); // Back to the default palette
        //        ui->undoColorSourceButton->setEnabled(true);

        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 142, 144);"));
        //currentColor->setColorSource(sourceCol);
    }
    else
    {
        isSourceColorChanged = false;
        ui->undoColorSourceButton->setVisible(false);
        // Make the button "invisible"
        //        QBrush tb(Qt::transparent); // Transparent brush, solid pattern
        //        ui->undoColorSourceButton->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb)); // Set every color roles to the transparent brush
        //        ui->undoColorSourceButton->setEnabled(false);

        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
    }
    ui->colorSourceText->setPlainText(ColorPair::toRGBA(sourceCol));
    updateSource(sourceCol);
}

void ColorWidget::changeDestinationColor(QColor destCol) {
    newDestColor = destCol;
    if(QString::compare(ColorPair::toRGBA(destCol),ColorPair::toRGBA(currentColor->getColorTarget())))
    {
        isDestColorChanged = true;
        ui->undoColorDestButton->setVisible(true);
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 142, 144);"));
    }
    else {
        isDestColorChanged = false;
        ui->undoColorDestButton->setVisible(false);
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
    }
    //currentColor->setColorTarget(destCol);
    ui->colorDestText->setPlainText(ColorPair::toRGBA(destCol));
    updateDestination(destCol);
}


void ColorWidget::on_colorDestButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->open(this, SLOT(changeDestinationColor(QColor)));
}

void ColorWidget::saveColor()
{
    if(currentColor != NULL)
    {
        if(isSourceColorChanged)
            currentColor->setColorSource(newSourceColor);
        if(isDestColorChanged)
            currentColor->setColorTarget(newDestColor);
        if(isColorIdChanged)
            currentColor->setId(newColorID);
        if(shouldDelete)
            emit deleteMe(currentColor);
    }
}


bool ColorWidget::getShouldDelete()
{
    return shouldDelete;
}

ColorPair* ColorWidget::getCurrentColorPair()
{
    return currentColor;
}


void ColorWidget::on_undoColorSourceButton_clicked()
{
    if(isSourceColorChanged)
    {
        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        ui->undoColorSourceButton->setVisible(false);
        newSourceColor = currentColor->getColorSource();
        ui->colorSourceText->setPlainText(ColorPair::toRGBA(newSourceColor));
        updateSource(newSourceColor);
        isSourceColorChanged = false;
    }
}


void ColorWidget::on_undoColorDestButton_clicked()
{
    if(isDestColorChanged)
    {
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        ui->undoColorDestButton->setVisible(false);
        newDestColor = currentColor->getColorTarget();
        ui->colorDestText->setPlainText(ColorPair::toRGBA(newDestColor));
        updateDestination(newDestColor);
        isDestColorChanged = false;
    }
}


void ColorWidget::on_deleteColorButton_clicked()
{
    shouldDelete = !shouldDelete;
    if(shouldDelete)
    {
        ui->deleteColorButton->setStyleSheet("border-image:url(:/icons/undo.png);");
    }
    else {
        ui->deleteColorButton->setStyleSheet("border-image:url(:/icons/delete.png);");
    }
}


void ColorWidget::on_undoColorIdButton_clicked()
{
    if(isColorIdChanged)
    {
        newColorID = currentColor->getId();
        ui->colorIdTextEdit->setText(newColorID);
        ui->undoColorIdButton->setVisible(false);
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        isColorIdChanged = false;
    }
}


void ColorWidget::on_colorIdTextEdit_textChanged()
{
    newColorID = ui->colorIdTextEdit->toPlainText();
    if(QString::compare(currentColor->getId(),newColorID)) {
        ui->undoColorIdButton->setVisible(true);
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 142, 144);"));
        isColorIdChanged = true;
    }
    else {
        ui->undoColorIdButton->setVisible(false);
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));
        isColorIdChanged = false;
    }
}

