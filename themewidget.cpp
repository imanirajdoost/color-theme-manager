#include "themewidget.h"
#include "ui_themewidget.h"
#include "ui/theme_editor_page.h"

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThemeWidget)
{
    ui->setupUi(this);
}

ThemeWidget::~ThemeWidget()
{
    delete ui;
}

void ThemeWidget::on_theme_button_clicked()
{
    std::cout << "Clicked on theme " << std::endl;
    //QPushButton* button = findChild<QPushButton*>("theme_button");
    //button->setText("testttttttt");
    theme_editor_page *wdg = new theme_editor_page;
    wdg->show();
    wdg->receiveThemeData(myTheme);
    connect(wdg,SIGNAL(sendNewThemeName(QString)),this,SLOT(updateThemeName(QString)));
//    wdg->setMainWindowRef(_mainWindow);
    // connect(this,SIGNAL(sendThemeData(Theme*)),wdg,SLOT(receiveThemeData(Theme*)));
    // emit sendThemeData(myTheme);

    // this->hide(); //this will disappear main window
}

//void ThemeWidget::setMainWindowRef(Ui::MainWindow* mainWin) {
//    _mainWindow = mainWin;
//}

void ThemeWidget::updateThemeName(QString newName)
{
    setText(newName);
}

void ThemeWidget::setThemeReference(Theme* _themeRef)
{
    std::cout << "setting Theme Ref" << std::endl;
    myTheme = _themeRef;
    setText(myTheme->themeName);

    QList<ColorPair*>* colors = _themeRef->getColorPair();
}

void ThemeWidget::setText(QString _buttonText)
{
    ui->theme_button->setText(_buttonText);
}

