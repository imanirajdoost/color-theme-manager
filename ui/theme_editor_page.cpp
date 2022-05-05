#include "theme_editor_page.h"
#include "ui_theme_editor_page.h"

theme_editor_page::theme_editor_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theme_editor_page)
{
    ui->setupUi(this);

    colorList = new QGridLayout;
    QList<ColorPair> listOfColors();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(colorList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget(scrollAreaContent);

    QHBoxLayout* colors_container = ui->colorLayout;
    colors_container->addLayout(colorList);
    colors_container->addWidget(scrollArea);
}

theme_editor_page::~theme_editor_page()
{
    delete ui;
}

void theme_editor_page::update_colors()
{

}

void theme_editor_page::receiveThemeData(Theme theme){
    ui->theme_name_text->setText(theme.themeName);
}
