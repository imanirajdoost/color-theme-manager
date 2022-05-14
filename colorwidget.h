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
    bool getShouldDelete();
    ColorPair* getCurrentColorPair();

private slots:
    void on_colorSourceButton_clicked();
    void changeSourceColor(QColor sourceCol);
    void changeDestinationColor(QColor destCol);

    void on_colorDestButton_clicked();


    void on_undoColorSourceButton_clicked();

    void on_undoColorDestButton_clicked();

    void on_deleteColorButton_clicked();

    void on_undoColorIdButton_clicked();

    void on_colorIdTextEdit_textChanged();

public slots:
    void saveColor();

signals:
    void deleteMe(ColorPair*);
    void setEditedColor(bool);

private:
    Ui::ColorWidget *ui;
    ColorPair* currentColor;
    void updateSource(QColor newColor);
    void updateDestination(QColor newColor);
    bool isChangesPresent();

    bool isSourceColorChanged = false;
    bool isDestColorChanged = false;
    bool isColorIdChanged = false;

    QColor newSourceColor;
    QColor newDestColor;
    QString newColorID;
    bool shouldDelete = false;
};

#endif // COLORWIDGET_H
