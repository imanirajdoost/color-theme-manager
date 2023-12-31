#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QWidget>
#include "src/theme.h"
#include <iostream>
#include <mainwindow.h>
#include <QHostInfo>
#include <QDesktopServices>

namespace Ui {
class ThemeWidget;
}

class ThemeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeWidget(QWidget *parent = nullptr);
    ~ThemeWidget();
    void setThemeReference(Theme* _themeRef);
    void setText(QString _buttonText);
//    void setMainWindowRef(Ui::MainWindow* mainWin);

public slots:
    void updateThemeName(QString newName);
    void updateThemeIcon(QString themeIcon);
    void getUpdatedTheme(Theme*);

private slots:
    void on_theme_button_clicked();

//signals:
    //void sendThemeData(Theme*);

    void on_pushButton_clicked();

    void on_deleteThemebutton_clicked();

signals:
    void updateTheme(Theme*);
    void deleteTheme(Theme*);

private:
    Ui::ThemeWidget *ui;
    Theme* myTheme;     //Theme pointer that is MINE!
    void showInGraphicalShell();
//    Ui::MainWindow* _mainWindow;
};

#endif // THEMEWIDGET_H
