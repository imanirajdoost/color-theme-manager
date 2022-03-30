#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/xmlreader.h"
#include <iostream>
#include <QFileDialog>
#include <QStringListModel>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->horizontalLayout->addWidget(new ThemeWidget(this));
    //scroll area
    //ui->scrollAreaWidgetContents->setLayout(new QVBoxLayout);
    //to add objects to scroll area
    //ui->scrollAreaWidgetContents->Layout()->addWidget(new ThemeWidget(this));
    //ui->scrollAreaWidgetContents->Layout()->addWidget(new ThemeWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));

    if(!fileName.isEmpty()) {
        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file);
    }
    else {
        std::cout << "ERROR: No file was found" << std::endl;
    }

    QStringListModel* model = new QStringListModel(this);
    //model->setStringList(list);
    //TODO: Add custom model for list
    ui->listView->setModel(model);

}

