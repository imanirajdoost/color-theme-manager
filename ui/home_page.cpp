#include "home_page.h"
#include "ui_home_page.h"

home_page::home_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::home_page)
{
    ui->setupUi(this);
}

home_page::~home_page()
{
    delete ui;
}
