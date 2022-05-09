#include "theme_editor_page.h"
#include "ui_theme_editor_page.h"

theme_editor_page::theme_editor_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theme_editor_page)
{
    ui->setupUi(this);

    colorList = new QGridLayout;
    //QList<ColorPair>* listOfColors();
    listOfColors = new QList<ColorPair*>();
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
    for(int i =0; i < listOfColors->size(); i++)
    {
        std::cout << "Color ID: " << listOfColors->at(i)->getId().toStdString() << std::endl;
        std::cout << "Color Source: " << ColorPair::toRGBA(listOfColors->at(i)->getColorSource()).toStdString() << std::endl;
        ColorWidget* colorWidget = new ColorWidget(this);
        colorWidget->setColorRef(listOfColors->at(i));
        colorList->addWidget(colorWidget);
    }
}

void theme_editor_page::receiveThemeData(Theme* theme){
    currentTheme = theme;
    // std::cout << "Theme name: " << currentTheme.themeName.toStdString() << std::endl;
    ui->theme_name_text->setText(currentTheme->themeName);
    listOfColors = currentTheme->getColorPair();
    update_colors();
}
