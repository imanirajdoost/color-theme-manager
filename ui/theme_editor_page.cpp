#include "theme_editor_page.h"
#include "ui_theme_editor_page.h"

theme_editor_page::theme_editor_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::theme_editor_page)
{
    ui->setupUi(this);
}

theme_editor_page::~theme_editor_page()
{
    delete ui;
}
