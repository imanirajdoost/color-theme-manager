#ifndef THEME_EDITOR_PAGE_H
#define THEME_EDITOR_PAGE_H

#include "src/colorpair.h"
#include "src/theme.h"
#include "colorwidget.h"
#include "src/xmlreader.h"

#include <fstream>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>
#include <QScrollArea>
#include <QGridLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QUrl>
#include <QUrl>
#include <mainwindow.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <regex>
#include <QCloseEvent>


namespace Ui {
class theme_editor_page;
}

class theme_editor_page : public QWidget
{
    Q_OBJECT

public:
    explicit theme_editor_page(QWidget *parent = nullptr);
    ~theme_editor_page();
    void receiveThemeData(Theme* myTheme);
    //    void setMainWindowRef(Ui::MainWindow* mainWin);

private slots:
    void on_addColorButton_clicked();
    void on_applyOnFileButton_clicked();

    void on_getFromWebButton_clicked();

    void on_themeNameEditText_textChanged();

    void on_saveThemeButton_clicked();

    void on_editIconButton_clicked();
    void downloadFinished();
    void downloadProgress(qint64 pr1, qint64 pr2);

    void on_undoThemeNameButton_clicked();

signals:
    void saveAllColors();
    void sendNewThemeName(QString newName);
    void sendNewThemeIcon(QString newIcon);
    void sendUpdatedTheme(Theme*);

private:
    Ui::theme_editor_page *ui;
    QList<ColorPair*>* listOfColors;
    QGridLayout* colorList;
    void update_colors();
    void add_message(QString m, QString color);
    void hideProgress();
    bool isUrlValid(QString url);
    void applyOnFile(QString fileName);
    bool isChangesPresent();
    Theme* currentTheme;
    bool isThemeNameChanged = false;
    bool isThemeIconChanged = false;
    bool isColorsChanged = false;
    QString newThemeIcon;

    //    Ui::MainWindow* _mainWindow;

public slots:
    void deleteColor(ColorPair* col);
    void setColorEdited(bool);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

};

#endif // THEME_EDITOR_PAGE_H
