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
    listOfThemes = new QList<Theme*>();
    QWidget* scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(themeList);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget (scrollAreaContent);
    scrollArea->setAlignment(Qt::AlignTop);

    QHBoxLayout* themes_container = ui->themeLayout;
    themes_container->addLayout(themeList);
    themes_container->addWidget(scrollArea);

    //Load the last session
    XMLReader xmlManager;
    stat = xmlManager.getLastStat();
    if(stat != NULL && stat->linksToFiles->count() > 0)
    {
        for(int i=0; i < stat->linksToFiles->count(); i++)
            openFile(stat->linksToFiles->at(i),false);
    }
}

void MainWindow::update_themes()
{
    for(int i =0; i < themeList->count(); i++) {
        themeList->itemAt(i)->widget()->deleteLater();
        themeList->removeWidget(themeList->itemAt(i)->widget());
    }
    for(int i =0; i < listOfThemes->size(); i++)
    {
        ThemeWidget* themeWidget = new ThemeWidget(this);
        themeWidget->setThemeReference(listOfThemes->at(i));
//        themeWidget->setMainWindowRef(this);
//        connect(this,SLOT(updateSingleTheme(Theme*)),themeWidget,SIGNAL(updateTheme(Theme*)));
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
        openFile(fileName,true);
    }
}

void MainWindow::on_button_import_theme_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), "", tr("XML files (*.xml)"));
    openFile(fileName,true);
}

void MainWindow::openFile(QString fileName, bool shouldWrite)
{
    if(!fileName.isEmpty()) {
        XMLReader reader;
        QFile file;
        file.setFileName(fileName);
        reader.read(file,listOfThemes);
        if(shouldWrite)
        {
        stat->linksToFiles->push_back(fileName);
        reader.write(stat);
        }
        update_themes();
    }
    else {
        add_error("No file or wrong extension was loaded");
        //std::cout << "ERROR: No file was found" << std::endl;
    }
}

void MainWindow::updateSingleTheme(Theme* _theme)
{
    update_themes();
}

void MainWindow::add_error(QString m)
{
    ui->text_error->appendPlainText("[" + QTime::currentTime().toString() + "] " + m);
}


void MainWindow::on_button_create_theme_clicked()
{
    ThemeWidget* themeWidget = new ThemeWidget(this);
    Theme* newTheme = new Theme();
    newTheme->setName("New Theme");
    ColorPair* newColorPair = new ColorPair("Couleur 0","#ffffffff","#ffffffff");
    newTheme->addColorPair(newColorPair);
    listOfThemes->push_back(newTheme);
    themeWidget->setThemeReference(listOfThemes->at(listOfThemes->size() - 1));
    themeList->addWidget(themeWidget);
}

