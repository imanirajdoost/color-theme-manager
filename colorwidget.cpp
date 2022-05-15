#include "colorwidget.h"
#include "ui_colorwidget.h"

#include <iostream>

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorWidget)
{
    ui->setupUi(this);

    isSourceColorChanged = false;
    isDestColorChanged =false;
    shouldDelete = false;
    isColorIdChanged = false;
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
    //    ui->undoColorSourceButton->setVisible(false);
    //    ui->undoColorDestButton->setVisible(false);
    //    ui->undoColorIdButton->setVisible(false);
    ui->undoColorSourceButton->setStyleSheet("border-image: url(:/icons/trans.png);");
    ui->undoColorDestButton->setStyleSheet("border-image: url(:/icons/trans.png);");
    ui->undoColorIdButton->setStyleSheet("border-image: url(:/icons/trans.png);");
    ui->willBeDeletedText->setStyleSheet("color: rgba(0, 0, 0, 0);");
    //    ui->willBeDeletedText->setVisible(false);
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
    colorDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
    colorDialog->open(this, SLOT(changeSourceColor(QColor)));
}

void ColorWidget::changeSourceColor(QColor sourceCol) {
    newSourceColor = sourceCol;
    if(QString::compare(ColorPair::toRGBA(sourceCol),ColorPair::toRGBA(currentColor->getColorSource())))
    {
        isSourceColorChanged = true;
        //        ui->undoColorSourceButton->setVisible(true);
        ui->undoColorSourceButton->setStyleSheet("border-image: url(:/icons/undo.png);");

        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 142, 144);\ncolor: rgb(46, 52, 54);"));
        //currentColor->setColorSource(sourceCol);
    }
    else
    {
        isSourceColorChanged = false;
        //        ui->undoColorSourceButton->setVisible(false);
        ui->undoColorSourceButton->setStyleSheet("border-image: url(:/icons/trans.png);");

        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
    }
    ui->colorSourceText->setPlainText(ColorPair::toRGBA(sourceCol));
    updateSource(sourceCol);
    emit setEditedColor(isChangesPresent());
}

void ColorWidget::changeDestinationColor(QColor destCol) {
    newDestColor = destCol;
    if(QString::compare(ColorPair::toRGBA(destCol),ColorPair::toRGBA(currentColor->getColorTarget())))
    {
        isDestColorChanged = true;
        //        ui->undoColorDestButton->setVisible(true);
        ui->undoColorDestButton->setStyleSheet("border-image: url(:/icons/undo.png);");
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 142, 144);\ncolor: rgb(46, 52, 54);"));
    }
    else {
        isDestColorChanged = false;
        //        ui->undoColorDestButton->setVisible(false);
        ui->undoColorDestButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
    }
    //currentColor->setColorTarget(destCol);
    ui->colorDestText->setPlainText(ColorPair::toRGBA(destCol));
    updateDestination(destCol);
    emit setEditedColor(isChangesPresent());
}


void ColorWidget::on_colorDestButton_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
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
        ui->colorSourceText->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
        //        ui->undoColorSourceButton->setVisible(false);
        ui->undoColorSourceButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        newSourceColor = currentColor->getColorSource();
        ui->colorSourceText->setPlainText(ColorPair::toRGBA(newSourceColor));
        updateSource(newSourceColor);
        isSourceColorChanged = false;
    }
    emit setEditedColor(isChangesPresent());
}


void ColorWidget::on_undoColorDestButton_clicked()
{
    if(isDestColorChanged)
    {
        ui->colorDestText->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
        //        ui->undoColorDestButton->setVisible(false);
        ui->undoColorDestButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        newDestColor = currentColor->getColorTarget();
        ui->colorDestText->setPlainText(ColorPair::toRGBA(newDestColor));
        updateDestination(newDestColor);
        isDestColorChanged = false;
    }
    emit setEditedColor(isChangesPresent());
}


void ColorWidget::on_deleteColorButton_clicked()
{
    shouldDelete = !shouldDelete;
    if(shouldDelete)
    {
        ui->deleteColorButton->setStyleSheet("border-image:url(:/icons/undo.png);");
        //        ui->willBeDeletedText->setVisible(true);
        ui->willBeDeletedText->setStyleSheet("color: rgba(0, 0, 0, 255);");
    }
    else {
        ui->deleteColorButton->setStyleSheet("border-image:url(:/icons/delete.png);");
        //        ui->willBeDeletedText->setVisible(false);
        ui->willBeDeletedText->setStyleSheet("color: rgba(0, 0, 0, 0);");
    }
    emit setEditedColor(isChangesPresent());
}


void ColorWidget::on_undoColorIdButton_clicked()
{
    if(isColorIdChanged)
    {
        newColorID = currentColor->getId();
        ui->colorIdTextEdit->setText(newColorID);
        //        ui->undoColorIdButton->setVisible(false);
        ui->undoColorIdButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
        isColorIdChanged = false;
    }
    emit setEditedColor(isChangesPresent());
}


void ColorWidget::on_colorIdTextEdit_textChanged()
{
    newColorID = ui->colorIdTextEdit->toPlainText();
    if(QString::compare(currentColor->getId(),newColorID)) {
        //        ui->undoColorIdButton->setVisible(true);
        ui->undoColorIdButton->setStyleSheet("border-image: url(:/icons/undo.png);");
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 142, 144);\ncolor: rgb(46, 52, 54);"));
        isColorIdChanged = true;
    }
    else {
        //        ui->undoColorIdButton->setVisible(false);
        ui->undoColorIdButton->setStyleSheet("border-image: url(:/icons/trans.png);");
        ui->colorIdTextEdit->setStyleSheet(QString("background-color: rgb(255, 255, 255);\ncolor: rgb(46, 52, 54);"));
        isColorIdChanged = false;
    }
    emit setEditedColor(isChangesPresent());
}

bool ColorWidget::isChangesPresent()
{
    if(isColorIdChanged || isSourceColorChanged || isDestColorChanged || shouldDelete)
        return true;
    return false;
}

