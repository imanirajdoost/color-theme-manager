#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // For drag and drops
    setAcceptDrops(true);
    // For adding scrolls for themes
    themeList = new QGridLayout;
    QList<Theme> listOfThemes();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(themeList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget (scrollAreaContent);

    QHBoxLayout* themes_container = ui->themeLayout;
    themes_container->addLayout(themeList);
    themes_container->addWidget(scrollArea);

    //TODO: Get theme numbers and create widgets according to this number
/*
    for(int i =0; i < 10; i++)
    {
        ThemeWidget* themeWidget = new ThemeWidget(this);
        themeList->addWidget(themeWidget);
    }
    */
}

void MainWindow::update_themes()
{
    for(int i =0; i < listOfThemes.size(); i++)
    {
        ThemeWidget* themeWidget = new ThemeWidget(this);
        themeWidget->setThemeReference(&(Theme)listOfThemes.at(i));
        themeList->addWidget(themeWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        openFile(fileName);
    }
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));
    openFile(fileName);
}

void MainWindow::openFile(QString fileName)
{
    if(!fileName.isEmpty()) {
        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file,listOfThemes);

        update_themes();
    }
    else {
        std::cout << "ERROR: No file was found" << std::endl;
    }
}

