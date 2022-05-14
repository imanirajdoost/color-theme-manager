#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "src/xmlreader.h"
#include "themewidget.h"

#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
    void update_themes();

private slots:
    void on_button_import_theme_clicked();
    void add_message(QString m,QString color);

    void on_button_create_theme_clicked();

    void on_saveThemeButton_clicked();

public slots:
    void updateSingleTheme(Theme* _theme);

private:
    QList<Theme*>* listOfThemes;
    QGridLayout* themeList;
    SystemStat* stat;

	Ui::MainWindow *ui;
    void openFile(QString fileName, QString iconPath);
protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
};
#endif // MAINWINDOW_H
