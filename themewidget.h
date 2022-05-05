#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QWidget>
#include "src/theme.h"
#include <iostream>

namespace Ui {
class ThemeWidget;
}

class ThemeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeWidget(QWidget *parent = nullptr);
    ~ThemeWidget();
    void setThemeReference(Theme *_themeRef);
    void setText(QString _labelText, QString _buttonText);

private slots:
    void on_theme_button_clicked();

//signals:
    //void sendThemeData(Theme*);

private:
    Ui::ThemeWidget *ui;
    Theme *myTheme;     //Theme pointer that is MINE!
};

#endif // THEMEWIDGET_H
