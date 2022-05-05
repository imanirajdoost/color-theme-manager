#ifndef THEME_EDITOR_PAGE_H
#define THEME_EDITOR_PAGE_H

#include "src/colorpair.h"
#include "src/theme.h"

#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QScrollArea>
#include <QGridLayout>

namespace Ui {
class theme_editor_page;
}

class theme_editor_page : public QWidget
{
    Q_OBJECT

public:
    explicit theme_editor_page(QWidget *parent = nullptr);
    ~theme_editor_page();
    void receiveThemeData(Theme myTheme);

private:
    Ui::theme_editor_page *ui;
    QList<ColorPair> listOfColors;
    QGridLayout* colorList;
    void theme_editor_page::update_colors();

//public slots:

};

#endif // THEME_EDITOR_PAGE_H
