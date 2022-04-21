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
    QPushButton* button = findChild<QPushButton*>("theme_button");
    button->setText("testttttttt");
    theme_editor_page *wdg = new theme_editor_page;
    wdg->show();
    // this->hide(); //this will disappear main window
}

void ThemeWidget::setThemeReference(Theme *_themeRef)
{
    std::cout << "setting Theme Ref" << std::endl;
    myTheme = _themeRef;
    setText("",myTheme->themeName);
}

void ThemeWidget::setText(QString _labelText, QString _buttonText)
{

}
