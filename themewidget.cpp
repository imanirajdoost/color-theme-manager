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
    connect(wdg,SIGNAL(sendNewThemeIcon(QString)),this,SLOT(updateThemeIcon(QString)));
    connect(wdg,SIGNAL(sendUpdatedTheme(Theme*)),this,SLOT(getUpdatedTheme(Theme*)));
    //    wdg->setMainWindowRef(_mainWindow);
    // connect(this,SIGNAL(sendThemeData(Theme*)),wdg,SLOT(receiveThemeData(Theme*)));
    // emit sendThemeData(myTheme);

    // this->hide(); //this will disappear main window
}

void ThemeWidget::getUpdatedTheme(Theme * upTheme)
{
    emit updateTheme(upTheme);
}

//void ThemeWidget::setMainWindowRef(Ui::MainWindow* mainWin) {
//    _mainWindow = mainWin;
//}

void ThemeWidget::updateThemeName(QString newName)
{
    setText(newName);
}

void ThemeWidget::updateThemeIcon(QString themeIcon)
{
    QImage image;
    if(image.load(themeIcon))
    {
        image.scaledToWidth(ui->theme_image->width(), Qt::SmoothTransformation);
        ui->theme_image->setPixmap(QPixmap::fromImage(image));
    }
}

void ThemeWidget::setThemeReference(Theme* _themeRef)
{
    //    std::cout << "setting Theme Ref" << std::endl;
    myTheme = _themeRef;
    setText(myTheme->themeName);
//    std::cout << "ICON PATH: " + myTheme->iconPath.toStdString() << std::endl;
    if(QString::compare(myTheme->iconPath,"") != 0)
    {
        updateThemeIcon(myTheme->iconPath);
    }
}

void ThemeWidget::setText(QString _buttonText)
{
    ui->theme_button->setText(_buttonText);
}

void ThemeWidget::showInGraphicalShell(QWidget *parent, const QString &pathIn)
{

}

