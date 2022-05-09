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
    // connect(this,SIGNAL(sendThemeData(Theme*)),wdg,SLOT(receiveThemeData(Theme*)));
    // emit sendThemeData(myTheme);

    // this->hide(); //this will disappear main window
}

void ThemeWidget::setThemeReference(Theme* _themeRef)
{
    std::cout << "setting Theme Ref" << std::endl;
    myTheme = _themeRef;
    setText("",myTheme->themeName);

    QList<ColorPair*>* colors = _themeRef->getColorPair();
        for(int i =0; i < colors->size(); i++)
        {
//            QColor currentColor= colors.at(i).getColorSource();
             //std::cout << currentColor.toRGBA(currentColor.getColorSource()) << std::endl;
        }

//    QList<ColorPair> colors = _themeRef.getColorPair();
//    for(int i =0; i < colors.size(); i++)
//    {
//        QColor c= (QColor)colors.at(i).getColorSource();
//        std::cout << colors.at(i).toRGBA(c) << std::endl;
//    }
}

void ThemeWidget::setText(QString _labelText, QString _buttonText)
{
    QPushButton* button = findChild<QPushButton*>("theme_button");
    button->setText(_buttonText);
}

