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
    theme_editor_page *wdg = new theme_editor_page;
    wdg->show();
    wdg->receiveThemeData(myTheme);
    connect(wdg,SIGNAL(sendNewThemeName(QString)),this,SLOT(updateThemeName(QString)));
    connect(wdg,SIGNAL(sendNewThemeIcon(QString)),this,SLOT(updateThemeIcon(QString)));
    connect(wdg,SIGNAL(sendUpdatedTheme(Theme*)),this,SLOT(getUpdatedTheme(Theme*)));
}

void ThemeWidget::getUpdatedTheme(Theme * upTheme)
{
    emit updateTheme(upTheme);
}

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

void ThemeWidget::showInGraphicalShell()
{
    QFileInfo info(myTheme->themePath);
    QDir directory(info.dir());
    QString path = info.absoluteDir().absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


void ThemeWidget::on_pushButton_clicked()
{
    showInGraphicalShell();
}


void ThemeWidget::on_deleteThemebutton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete theme?",
                                  "Are you sure you want to delete this theme?\n"
                                  "The action is cannot be undone",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit deleteTheme(myTheme);
    }
}

